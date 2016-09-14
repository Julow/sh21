/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 18:55:49 by juloo             #+#    #+#             */
/*   Updated: 2016/09/14 15:11:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_destroy_cmd.h"
#include <stdlib.h>

void			sh_destroy_redir_lst(t_sh_redir_lst *lst)
{
	t_sh_redir		*r;

	r = VECTOR_IT(lst->redirs);
	while (VECTOR_NEXT(lst->redirs, r))
	{
		if (r->type == SH_REDIR_STRING)
			sh_destroy_text(r->here_string);
		else
			sh_destroy_text(&r->right_text);
	}
	ft_vclear(&lst->redirs);
}

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
