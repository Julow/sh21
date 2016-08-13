/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_for_clause.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 16:04:33 by juloo             #+#    #+#             */
/*   Updated: 2016/08/13 19:21:40 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

#define SUB_DST(DST, S)		SUB(ft_memcpy(DST, (S).str, (S).length), (S).length)

static bool		is_identifier(t_sub str)
{
	uint32_t		i;

	if (str.length < 1 || !IS(str.str[0], IS_ALPHA | IS_UNDERSCORE))
		return (false);
	i = 1;
	while (i < str.length)
	{
		if (!IS(str.str[i], IS_WORD))
			return (false);
		i++;
	}
	return (true);
}

static bool		sh_except_token_str(t_sh_parser *p, t_sub str) // TODO: .default token
{
	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	if (SH_T(p) != NULL || (str.length > 0 && !SUB_EQU(str, p->l.t.token)))
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	return (true);
}

bool			sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	sh_ignore_newlines(p);
	if (!sh_except_token_str(p, SUB0()))
		return (false);
	if (!is_identifier(p->l.t.token))
		return (sh_parse_error(p, SH_E_INVALID_ID));
	dst->for_clause = MALLOC(sizeof(t_sh_for) + p->l.t.token.length);
	dst->for_clause->var = SUB_DST(ENDOF(dst->for_clause), p->l.t.token);
	sh_ignore_newlines(p);
	if (sh_except_token_str(p, SUBC("in")))
	{
		sh_ignore_newlines(p);
		dst->for_clause->data = SH_TEXT();
		if (sh_parse_text(p, &dst->for_clause->data))
		{
			if (!sh_parse_compound_end(p))
				sh_parse_error(p, SH_E_UNEXPECTED);
			else if (sh_parse_do_clause(p, &dst->for_clause->body))
				return (true);
		}
		sh_destroy_text(&dst->for_clause->data);
	}
	free(dst->for_clause);
	return (false);
}
