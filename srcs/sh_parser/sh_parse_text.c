/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:24:33 by juloo             #+#    #+#             */
/*   Updated: 2016/08/28 02:04:29 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_subst_subshell(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sh_compound *const	cmd = NEW(t_sh_compound);
	t_lexer_state const		*prev;
	bool					r;

	ft_lexer_push(&p->l, &prev);
	if ((r = sh_parse_compound(p, cmd, true)
			&& (!p->l.eof || sh_parse_error(p, SH_E_EOF))
			&& (SH_T(p)->type == SH_PARSE_T_COMPOUND_END || sh_parse_error(p, SH_E_ERROR))
			&& (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_SUBSHELL)
		))
		sh_text_push(dst, SUB0(), SH_TOKEN(SUBSHELL, .cmd=cmd), quoted);
	else
		free(cmd);
	ft_lexer_pop(&p->l, prev);
	return (r);
}

static bool		sh_parse_subst_expr(t_sh_parser *p, t_sh_text *dst, bool quoted) { return (ASSERT(false)); (void)p; (void)dst; (void)quoted; }
static bool		sh_parse_subst_math(t_sh_parser *p, t_sh_text *dst, bool quoted) { return (ASSERT(false)); (void)p; (void)dst; (void)quoted; }

// TODO: special params ($@, $#, $!, ...) (token?)
static bool		sh_parse_subst_dollar(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sub			str;
	uint32_t		i;

	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	if (SH_T(p)->type != SH(TEXT))
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	str = p->l.t.token_str;
	if (IS(str.str[0], IS_DIGIT))
	{
		i = 1;
		sh_text_push(dst, SUB0(),
			SH_TOKEN(PARAM_POS, .param_pos=str.str[0] - '0'), quoted);
	}
	else
	{
		i = 0;
		while (i < str.length && IS(str.str[i], IS_WORD))
			i++;
		if (i == 0)
			sh_text_push_string(dst, SUBC("$"), quoted);
		else
			sh_text_push(dst, SUB_LEN(str, i),
				SH_TOKEN(PARAM, .param_len=i), quoted);
	}
	if (i < str.length)
		sh_text_push_string(dst, SUB_FOR(str, i), quoted);
	return (true);
}

static bool		(*const g_sh_parse_subst[])(t_sh_parser*, t_sh_text*, bool) = {
	[SH_PARSE_T_SUBST_EXPR] = &sh_parse_subst_expr,
	[SH_PARSE_T_SUBST_MATH] = &sh_parse_subst_math,
	[SH_PARSE_T_SUBST_SUBSHELL] = &sh_parse_subst_subshell,
	[SH_PARSE_T_SUBST_DOLLAR] = &sh_parse_subst_dollar,
};

static bool		sh_parse_text_string(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_lexer_state const		*prev;

	ft_lexer_push(&p->l, &prev);
	while (ft_lexer_next(&p->l))
		if (SH_T_EQU(p, STRING, STRING_END))
			break ;
		else if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			if (!g_sh_parse_text[SH_T(p)->type](p, dst, true))
				return (false);
		}
		else
			return (sh_parse_error(p, SH_E_ERROR));
	ft_lexer_pop(&p->l, prev);
	return (true);
	(void)quoted;
}

static bool		sh_parse_text_comment(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sh_parse_token const	*t;
	t_lexer_state const		*prev;

	ft_lexer_push(&p->l, &prev);
	while (true)
	{
		if (!ft_lexer_ahead(&p->l, NULL, V(&t))
			|| t->type == SH_PARSE_T_COMMENT)
			break ;
		if (!ft_lexer_next(&p->l))
			ASSERT(false);
	}
	ft_lexer_pop(&p->l, prev);
	return (true);
	(void)dst;
	(void)quoted;
}

static bool		sh_parse_text_backslash(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sh_parse_token const	*t;

	TRACE();
	if (!ft_lexer_ahead(&p->l, NULL, V(&t)))
		return (sh_parse_error(p, SH_E_EOF));
	if (t->type == SH(COMPOUND_END) && t->compound_end == SH(COMPOUND_NEWLINE))
		ft_lexer_next(&p->l); // TODO: '\\\n' token 'ESCAPED'
	else
		sh_text_push_string(dst, SUB0(), true);
	return (true);
	(void)quoted;
}

static bool		sh_parse_text_text(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	sh_text_push_string(dst, p->l.t.token_str, quoted);
	return (true);
}

static bool		sh_parse_text_space(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	ASSERT(!quoted);
	sh_text_push(dst, SUB0(), SH_TOKEN(SPACE, 0), false);
	return (true);
	(void)p;
}

static bool		sh_parse_text_subst(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	return (g_sh_parse_subst[SH_T(p)->subst](p, dst, quoted));
}

static bool		sh_parse_text_escaped(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	sh_text_push_string(dst, SUB(&SH_T(p)->escaped, 1), true);
	return (true);
	(void)quoted;
}

bool			(*const g_sh_parse_text[_SH_PARSE_T_COUNT_])(t_sh_parser *p, t_sh_text *dst, bool quoted) = {
	[SH_PARSE_T_SPACE] = &sh_parse_text_space,
	[SH_PARSE_T_TEXT] = &sh_parse_text_text,
	[SH_PARSE_T_SUBST] = &sh_parse_text_subst,
	[SH_PARSE_T_STRING] = &sh_parse_text_string,
	[SH_PARSE_T_COMMENT] = &sh_parse_text_comment,
	[SH_PARSE_T_BACKSLASH] = &sh_parse_text_backslash,
	[SH_PARSE_T_ESCAPED] = &sh_parse_text_escaped,
	[SH_PARSE_T_ESCAPE_SEQUENCE] = &sh_parse_text_escape_sequence,
};
