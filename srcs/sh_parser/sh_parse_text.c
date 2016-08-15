/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:24:33 by juloo             #+#    #+#             */
/*   Updated: 2016/08/15 15:50:30 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static void		push_token(t_sh_text *text, t_sub str, t_sh_token t, bool quoted)
{
	if (quoted)
		t.type |= SH_F_T_QUOTED;
	ft_dstradd(&text->text, str);
	ft_vpush(&text->tokens, &t, 1);
}

static void		push_token_string(t_sh_text *text, t_sub str, bool quoted)
{
	t_sh_token		token;

	token = SH_TOKEN(STRING, .token_len=str.length);
	push_token(text, str, token, quoted);
}

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
		push_token(dst, SUB0(), SH_TOKEN(SUBSHELL, .cmd=cmd), quoted);
	else
		free(cmd);
	ft_lexer_pop(&p->l, prev);
	return (r);
}

static bool		sh_parse_subst_expr(t_sh_parser *p, t_sh_text *dst, bool quoted) { return (ASSERT(false)); (void)p; (void)dst; (void)quoted; }
static bool		sh_parse_subst_math(t_sh_parser *p, t_sh_text *dst, bool quoted) { return (ASSERT(false)); (void)p; (void)dst; (void)quoted; }

static bool		sh_parse_subst_dollar(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sub			str;
	uint32_t		i;

	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	str = p->l.t.token;
	if (p->l.token == NULL && IS(str.str[0], IS_DIGIT))
	{
		i = 1;
		push_token(dst, SUB0(),
			SH_TOKEN(PARAM_POS, .param_pos=str.str[0] - '0'), quoted);
	}
	else
	{
		i = 0;
		if (p->l.token == NULL)
			while (i < str.length && IS(str.str[i], IS_WORD))
				i++;
		// TODO: special params ($@, $#, $!, ...) (token?)
		push_token(dst, (i == 0) ? SUBC("$") : SUB_LEN(str, i),
			SH_TOKEN(PARAM, .param_len=i), quoted);
	}
	if (i < str.length)
		push_token_string(dst, SUB_FOR(str, i), quoted);
	return (true);
}

static bool		(*const g_sh_parse_subst[])(t_sh_parser*, t_sh_text*, bool) = {
	[SH_PARSE_T_SUBST_EXPR] = &sh_parse_subst_expr,
	[SH_PARSE_T_SUBST_MATH] = &sh_parse_subst_math,
	[SH_PARSE_T_SUBST_SUBSHELL] = &sh_parse_subst_subshell,
	[SH_PARSE_T_SUBST_DOLLAR] = &sh_parse_subst_dollar,
};

static bool		sh_parse_text_string(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;
	t_lexer_state const		*prev;
	bool					r;

	ft_lexer_push(&p->l, &prev);
	r = true;
	while (ft_lexer_next(&p->l))
	{
		if ((t = SH_T(p)) == NULL)
			push_token_string(dst, p->l.t.token, true);
		else if (t->type == SH_PARSE_T_SUBST)
			r = g_sh_parse_subst[t->subst](p, dst, true);
		else if (t->type == SH_PARSE_T_ESCAPED)
			push_token_string(dst, SUB_FOR(p->l.t.token, 1), true);
		else if (t->type == SH_PARSE_T_STRING)
		{
			ASSERT(t->string == SH_PARSE_T_STRING_END);
			break ;
		}
		else
			ASSERT(false, "Invalid token");
		if (!r)
			return (false);
	}
	ft_lexer_pop(&p->l, prev);
	return (true);
}

static bool		sh_parse_text_comment(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;
	t_lexer_state const		*prev;

	ft_lexer_push(&p->l, &prev);
	while (true)
	{
		if (!ft_lexer_ahead(&p->l, NULL, V(&t))
			|| (t != NULL && t->type == SH_PARSE_T_COMMENT))
			break ;
		if (!ft_lexer_next(&p->l))
			ASSERT(false);
	}
	ft_lexer_pop(&p->l, prev);
	return (true);
	(void)dst;
}

static bool		sh_parse_text_backslash(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;

	TRACE();
	if (!ft_lexer_ahead(&p->l, NULL, V(&t)))
		return (sh_parse_error(p, SH_E_EOF));
	if (t->type == SH(COMPOUND_END) && t->compound_end == SH(COMPOUND_NEWLINE))
		ft_lexer_next(&p->l); // TODO: '\\\n' token 'IGNORE'
	else
		push_token_string(dst, SUB0(), true);
	return (true);
}

static bool		sh_parse_text_text(t_sh_parser *p, t_sh_text *dst)
{
	push_token_string(dst, p->l.t.token, false);
	return (true);
}

static bool		sh_parse_text_space(t_sh_parser *p, t_sh_text *dst)
{
	push_token(dst, SUB0(), SH_TOKEN(SPACE, 0), false);
	return (true);
	(void)p;
}

static bool		sh_parse_text_subst(t_sh_parser *p, t_sh_text *dst)
{
	return (g_sh_parse_subst[SH_T(p)->subst](p, dst, false));
}

static bool		sh_parse_text_escaped(t_sh_parser *p, t_sh_text *dst)
{
	push_token_string(dst, SUB_FOR(p->l.t.token, 1), true);
	return (true);
}

bool			(*const g_sh_parse_text[_SH_PARSE_T_COUNT_])(t_sh_parser *p, t_sh_text *dst) = {
	[SH_PARSE_T_SPACE] = &sh_parse_text_space,
	[SH_PARSE_T_TEXT] = &sh_parse_text_text,
	[SH_PARSE_T_SUBST] = &sh_parse_text_subst,
	[SH_PARSE_T_STRING] = &sh_parse_text_string,
	[SH_PARSE_T_COMMENT] = &sh_parse_text_comment,
	[SH_PARSE_T_BACKSLASH] = &sh_parse_text_backslash,
	[SH_PARSE_T_ESCAPED] = &sh_parse_text_escaped,
};
