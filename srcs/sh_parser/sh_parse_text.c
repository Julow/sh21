/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:24:33 by juloo             #+#    #+#             */
/*   Updated: 2016/09/08 18:23:45 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_subst_math(t_sh_parser *p, t_sh_text *dst, bool quoted) { return (ASSERT(false)); (void)p; (void)dst; (void)quoted; }

static bool		(*const g_sh_parse_subst[])(t_sh_parser*, t_sh_text*, bool) = {
	[SH_PARSE_T_SUBST_PARAM] = &sh_parse_text_subst_param,
	[SH_PARSE_T_SUBST_MATH] = &sh_parse_subst_math,
};

static bool		sh_parse_text_param(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sub const			param_str = SUB_FOR(p->l.t.token_str, SH_T(p)->param_prefix);
	t_sh_param const	param = SH_PARAM(STR, .str_length=param_str.length);

	sh_text_push(dst, param_str, SH_TOKEN(PARAM, .param=param), quoted);
	return (true);
}

static bool		sh_parse_text_param_pos(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	uint32_t			val = p->l.t.token_str.str[SH_T(p)->param_prefix] - '0';
	t_sh_param const	param = SH_PARAM(POS, .pos=val);

	sh_text_push(dst, SUB0(), SH_TOKEN(PARAM, .param=param), quoted);
	return (true);
}

static bool		sh_parse_text_param_special(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sh_param const	param = SH_PARAM(SPECIAL, .special=SH_T(p)->param_special);

	sh_text_push(dst, SUB0(), SH_TOKEN(PARAM, .param=param), quoted);
	return (true);
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
	[SH_PARSE_T_PARAM] = &sh_parse_text_param,
	[SH_PARSE_T_PARAM_POS] = &sh_parse_text_param_pos,
	[SH_PARSE_T_PARAM_SPECIAL] = &sh_parse_text_param_special,
	[SH_PARSE_T_SUBST] = &sh_parse_text_subst,
	[SH_PARSE_T_SUBSHELL] = &sh_parse_text_subshell,
	[SH_PARSE_T_STRING] = &sh_parse_text_string,
	[SH_PARSE_T_COMMENT] = &sh_parse_text_comment,
	[SH_PARSE_T_BACKSLASH] = &sh_parse_text_backslash,
	[SH_PARSE_T_ESCAPED] = &sh_parse_text_escaped,
	[SH_PARSE_T_ESCAPE_SEQUENCE] = &sh_parse_text_escape_sequence,
};
