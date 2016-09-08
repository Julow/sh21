/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_while_clause.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 10:48:16 by juloo             #+#    #+#             */
/*   Updated: 2016/09/08 18:59:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_while_cond(t_sh_parser *p, t_sh_cmd *dst)
{
	return ((!p->l.eof || sh_parse_error(p, SH_E_EOF))
		&& sh_parse_compound(p, &dst->while_clause->cond, true)
		&& ((sh_parse_do_clause(p, &dst->while_clause->body)
			&& (sh_parse_trailing_redirs(p, &dst->redirs)
				|| (sh_destroy_compound(&dst->while_clause->body), false)))
			|| (sh_destroy_compound(&dst->while_clause->cond), false)));
}

bool			sh_parse_while_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	if (!sh_ignore_newlines(p))
		return (sh_parse_error(p, SH_E_EOF));
	dst->while_clause = NEW(t_sh_while);
	if (!sh_parse_while_cond(p, dst))
	{
		free(dst->while_clause);
		return (false);
	}
	return (true);
}
