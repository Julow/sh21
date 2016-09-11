/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_if_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 10:43:46 by juloo             #+#    #+#             */
/*   Updated: 2016/09/11 18:43:15 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		parse_if(t_sh_parser *p, t_sh_if *dst, bool elif);

static bool		parse_if_end(t_sh_parser *p, t_sh_else **dst)
{
	*dst = NEW(t_sh_else);
	if (SH_T_EQU(p, COMPOUND_END, COMPOUND_ELIF))
	{
		(*dst)->type = SH_ELSE_ELIF;
		if (parse_if(p, &(*dst)->elif_clause, true))
			return (true);
	}
	else if (SH_T_EQU(p, COMPOUND_END, COMPOUND_ELSE))
	{
		(*dst)->type = SH_ELSE_ELSE;
		if (!sh_ignore_newlines(p))
			sh_parse_error_unterminated(p, SH_E_UNTERMINATED_ELSE);
		else if (sh_parse_compound(p, &(*dst)->else_clause, true))
		{
			if (p->t.eof)
				sh_parse_error_unterminated(p, SH_E_UNTERMINATED_ELSE);
			else if (SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_FI))
				return (true);
			else
				sh_parse_error(p, SH_E_UNEXPECTED);
			sh_destroy_compound(&(*dst)->else_clause);
		}
	}
	else
		sh_parse_error(p, SH_E_UNEXPECTED);
	free(*dst);
	return (false);
}

static bool		parse_if(t_sh_parser *p, t_sh_if *dst, bool elif)
{
	if (!sh_ignore_newlines(p))
		return (sh_parse_error_unterminated(p,
				elif ? SH_E_UNTERMINATED_ELIF : SH_E_UNTERMINATED_IF));
	if (sh_parse_compound(p, &dst->cond, true))
	{
		if (p->t.eof)
			sh_parse_error_unterminated(p,
					elif ? SH_E_UNTERMINATED_ELIF : SH_E_UNTERMINATED_IF);
		else if (!SH_T_EQU(p, COMPOUND_END, COMPOUND_THEN))
			sh_parse_error(p, SH_E_UNEXPECTED);
		else if (!sh_ignore_newlines(p))
			sh_parse_error_unterminated(p, SH_E_UNTERMINATED_THEN);
		else if (sh_parse_compound(p, &dst->body, true))
		{
			dst->else_clause = NULL;
			if (p->t.eof)
				sh_parse_error_unterminated(p, SH_E_UNTERMINATED_THEN);
			else if (SH_T_EQU(p, COMPOUND_END, COMPOUND_FI)
				|| parse_if_end(p, &dst->else_clause))
				return (true);
			sh_destroy_compound(&dst->body);
		}
		sh_destroy_compound(&dst->cond);
	}
	return (false);
}

bool			sh_parse_if_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->if_clause = NEW(t_sh_if);
	if (parse_if(p, dst->if_clause, false))
	{
		if (sh_parse_trailing_redirs(p, &dst->redirs))
			return (true);
		sh_destroy_if_clause(dst->if_clause);
	}
	free(dst->if_clause);
	return (false);
}
