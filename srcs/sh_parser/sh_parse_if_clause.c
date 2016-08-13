/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_if_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 10:43:46 by juloo             #+#    #+#             */
/*   Updated: 2016/08/12 18:22:57 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_if_cond(t_sh_parser *p, t_sh_if *if_clause);

static bool		sh_parse_if_else(t_sh_parser *p, t_sh_else **dst, bool elif)
{
	sh_ignore_newlines(p);
	*dst = NEW(t_sh_else);
	if (elif)
	{
		(*dst)->type = SH_ELSE_ELIF;
		if (sh_parse_if_cond(p, &(*dst)->elif_clause))
			return (true);
	}
	else
	{
		(*dst)->type = SH_ELSE_ELSE;
		if (sh_parse_compound(p, &(*dst)->else_clause, true))
		{
			if (!SH_T_EQU(p, COMPOUND_END)
				|| SH_T(p)->compound_end != SH_PARSE_T_COMPOUND_FI)
				sh_parse_error(p, SH_E_UNEXPECTED);
			else
				return (true);
		}
	}
	free(*dst);
	return (false);
}

static bool		sh_parse_if_end(t_sh_parser *p, t_sh_else **dst)
{
	if (p->l.eof)
		return (sh_parse_error(p, SH_E_EOF));
	if (!SH_T_EQU(p, COMPOUND_END))
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_ELSE)
		return (sh_parse_if_else(p, dst, false));
	else if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_ELIF)
		return (sh_parse_if_else(p, dst, true));
	else if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_FI)
	{
		*dst = NULL;
		return (true);
	}
	else
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	return (false);
}

static bool		sh_parse_if_then(t_sh_parser *p, t_sh_if *if_clause)
{
	return ((!p->l.eof || sh_parse_error(p, SH_E_EOF))
		&& ((SH_T_EQU(p, COMPOUND_END)
			&& SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_THEN)
				|| sh_parse_error(p, SH_E_UNEXPECTED))
		&& (sh_ignore_newlines(p) || sh_parse_error(p, SH_E_EOF))
		&& sh_parse_compound(p, &if_clause->body, true)
		&& (sh_parse_if_end(p, &if_clause->else_clause)
			|| (sh_destroy_compound(&if_clause->body), false)));
}

static bool		sh_parse_if_cond(t_sh_parser *p, t_sh_if *if_clause)
{
	return ((!p->l.eof || sh_parse_error(p, SH_E_EOF))
		&& sh_parse_compound(p, &if_clause->cond, true)
		&& (sh_parse_if_then(p, if_clause)
			|| (sh_destroy_compound(&if_clause->cond), false)));
}

bool			sh_parse_if_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	if (!sh_ignore_newlines(p))
		return (sh_parse_error(p, SH_E_EOF));
	dst->if_clause = NEW(t_sh_if);
	if (!sh_parse_if_cond(p, dst->if_clause))
	{
		free(dst->if_clause);
		return (false);
	}
	return (true);
}
