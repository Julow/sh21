/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 18:55:49 by juloo             #+#    #+#             */
/*   Updated: 2016/07/28 18:05:19 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_destroy_cmd.h"

void			sh_destroy_if_clause(t_sh_if *c)
{
	t_sh_else			*else_clause;

	sh_destroy_text(&c->text);
	sh_destroy_compound(&c->cond);
	sh_destroy_compound(&c->body);
	if ((else_clause = c->else_clause) != NULL)
	{
		if (else_clause->type == SH_ELSE_ELIF)
			sh_destroy_if_clause(&else_clause->c.elif_clause);
		else
			sh_destroy_compound(&else_clause->c.else_clause);
	}
}

void			sh_destroy_for_clause(t_sh_for *c)
{
	sh_destroy_text(&c->text);
	sh_destroy_text(&c->data);
	sh_destroy_compound(&c->body);
}

void			sh_destroy_while_clause(t_sh_while *c)
{
	sh_destroy_text(&c->text);
	sh_destroy_compound(&c->cond);
	sh_destroy_compound(&c->body);
}
