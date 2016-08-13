/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_while_clause.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 10:48:16 by juloo             #+#    #+#             */
/*   Updated: 2016/08/12 21:20:58 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_while_cond(t_sh_parser *p, t_sh_while *dst)
{
	return ((!p->l.eof || sh_parse_error(p, SH_E_EOF))
		&& sh_parse_compound(p, &dst->cond, true)
		&& (sh_parse_do_clause(p, &dst->body)
			|| (sh_destroy_compound(&dst->cond), false)));
}

bool			sh_parse_while_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	if (!sh_ignore_newlines(p))
		return (sh_parse_error(p, SH_E_EOF));
	dst->while_clause = NEW(t_sh_while);
	if (!sh_parse_while_cond(p, dst->while_clause))
	{
		free(dst->while_clause);
		return (false);
	}
	return (true);
}
