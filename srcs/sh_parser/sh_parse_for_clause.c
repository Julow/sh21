/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_for_clause.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 16:04:33 by juloo             #+#    #+#             */
/*   Updated: 2016/09/09 18:40:33 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "p_sh_parser.h"

#include <stdlib.h>

static bool		parse_for_clause_data(t_sh_parser *p, t_sh_text *dst)
{
	while (ft_tokenize(&p->t) && g_sh_parse_text[SH_T(p)->type] != NULL)
		if (!g_sh_parse_text[SH_T(p)->type](p, dst, false))
			return (false);
	return (true);
}

bool			sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	sh_ignore_newlines(p);
	if (!sh_except_token(p, SH_PARSE_T(TEXT)))
		return (false);
	if (!ft_subis_identifier(p->t.token_str))
		return (sh_parse_error(p, SH_E_INVALID_ID));
	dst->for_clause = MALLOC(sizeof(t_sh_for) + p->t.token_str.length);
	dst->for_clause->var = SUB_DST(ENDOF(dst->for_clause), p->t.token_str);
	sh_ignore_newlines(p);
	if (sh_except_token(p, SH_PARSE_T(TEXT))
		&& (SUB_EQU(p->t.token_str, SUBC("in"))
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
