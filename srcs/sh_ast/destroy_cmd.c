/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 18:55:49 by juloo             #+#    #+#             */
/*   Updated: 2016/08/12 18:20:47 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_destroy_cmd.h"
#include <stdlib.h>

void			sh_destroy_if_clause(t_sh_if *c)
{
	sh_destroy_compound(&c->cond);
	sh_destroy_compound(&c->body);
	if (c->else_clause != NULL)
	{
		if (c->else_clause->type == SH_ELSE_ELIF)
			sh_destroy_if_clause(&c->else_clause->elif_clause);
		else
			sh_destroy_compound(&c->else_clause->else_clause);
		free(c->else_clause);
	}
}

void			sh_destroy_for_clause(t_sh_for *c)
{
	sh_destroy_text(&c->data);
	sh_destroy_compound(&c->body);
}

void			sh_destroy_while_clause(t_sh_while *c)
{
	sh_destroy_compound(&c->cond);
	sh_destroy_compound(&c->body);
}
