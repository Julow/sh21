/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_for_clause.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 16:04:33 by juloo             #+#    #+#             */
/*   Updated: 2016/08/18 15:51:18 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

#define SUB_DST(DST, S)		SUB(ft_memcpy(DST, (S).str, (S).length), (S).length)

static bool		is_identifier(t_sub str)
{
	return (str.length >= 1 && IS(str.str[0], IS_ALPHA | IS_UNDERSCORE)
		&& ft_subis(SUB_FOR(str, 1), IS_WORD));
}

static bool		parse_for_clause_data(t_sh_parser *p, t_sh_text *dst)
{
	while (ft_lexer_next(&p->l) && g_sh_parse_text[SH_T(p)->type] != NULL)
		if (!g_sh_parse_text[SH_T(p)->type](p, dst))
			return (false);
	return (true);
}

bool			sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	sh_ignore_newlines(p);
	if (!sh_except_token(p, SH_PARSE_T(TEXT)))
		return (false);
	if (!is_identifier(p->l.t.token))
		return (sh_parse_error(p, SH_E_INVALID_ID));
	dst->for_clause = MALLOC(sizeof(t_sh_for) + p->l.t.token.length);
	dst->for_clause->var = SUB_DST(ENDOF(dst->for_clause), p->l.t.token);
	sh_ignore_newlines(p);
	if (sh_except_token(p, SH_PARSE_T(TEXT))
		&& (SUB_EQU(p->l.t.token, SUBC("in"))
			|| sh_parse_error(p, SH_E_UNEXPECTED)))
	{
		sh_ignore_newlines(p);
		dst->for_clause->data = SH_TEXT();
		if (parse_for_clause_data(p, &dst->for_clause->data))
		{
			if (!sh_parse_compound_end(p))
				sh_parse_error(p, SH_E_UNEXPECTED);
			else if (sh_parse_do_clause(p, &dst->for_clause->body))
			{
				if (sh_parse_trailing_redirs(p, &dst->redirs))
					return (true);
				sh_destroy_compound(&dst->for_clause->body);
			}
		}
		sh_destroy_text(&dst->for_clause->data);
	}
	free(dst->for_clause);
	return (false);
}
