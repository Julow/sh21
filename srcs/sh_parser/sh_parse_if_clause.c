/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_if_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 10:43:46 by juloo             #+#    #+#             */
/*   Updated: 2016/08/18 15:46:07 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		parse_if(t_sh_parser *p, t_sh_if *dst);

static bool		parse_if_then(t_sh_parser *p, t_sh_compound *dst)
{
	if (!SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_THEN))
		return (false);
	sh_ignore_newlines(p);
	return (sh_parse_compound(p, dst, true));
}

static bool		parse_if_end(t_sh_parser *p, t_sh_else **dst)
{
	*dst = NEW(t_sh_else);
	if (SH_T_EQU(p, COMPOUND_END, COMPOUND_ELIF))
	{
		(*dst)->type = SH_ELSE_ELIF;
		if (parse_if(p, &(*dst)->elif_clause))
			return (true);
	}
	else if (SH_T_EQU(p, COMPOUND_END, COMPOUND_ELSE))
	{
		(*dst)->type = SH_ELSE_ELSE;
		sh_ignore_newlines(p);
		if (sh_parse_compound(p, &(*dst)->else_clause, true))
		{
			if (SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_FI))
				return (true);
			sh_parse_error(p, SH_E_UNEXPECTED);
			sh_destroy_compound(&(*dst)->else_clause);
		}
	}
	else
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	free(*dst);
	return (false);
}

static bool		parse_if(t_sh_parser *p, t_sh_if *dst)
{
	sh_ignore_newlines(p);
	if (sh_parse_compound(p, &dst->cond, true))
	{
		if (parse_if_then(p, &dst->body))
		{
			if (!p->l.eof)
			{
				dst->else_clause = NULL;
				if (SH_T_EQU(p, COMPOUND_END, COMPOUND_FI)
					|| parse_if_end(p, &dst->else_clause))
					return (true);
			}
			else
				sh_parse_error(p, SH_E_EOF);
			sh_destroy_compound(&dst->body);
		}
		sh_destroy_compound(&dst->cond);
	}
	return (false);
}

bool			sh_parse_if_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->if_clause = NEW(t_sh_if);
	if (parse_if(p, dst->if_clause))
	{
		if (sh_parse_trailing_redirs(p, &dst->redirs))
			return (true);
		sh_destroy_if_clause(dst->if_clause);
	}
	free(dst->if_clause);
	return (false);
}
