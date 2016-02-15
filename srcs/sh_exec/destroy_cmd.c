/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/14 12:42:55 by juloo             #+#    #+#             */
/*   Updated: 2016/02/15 18:11:21 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/cmd.h"
#include <stdlib.h>

void			sh_destroy_text(t_sh_text *text)
{
	uint32_t		i;
	t_sh_token		*t;

	ft_dstrclear(&text->text);
	i = 0;
	while (i < text->tokens.length)
	{
		t = VECTOR_GET(text->tokens, i++);
		if (t->type == SH_T_SUBSHELL)
			sh_destroy_cmd(t->val.cmd);
		else if (t->type == SH_T_EXPR)
			sh_destroy_expr(t->val.expr);
	}
	ft_vclear(&text->tokens);
}

void			sh_destroy_expr(t_sh_expr *expr)
{
	sh_destroy_text(&expr->text);
	free(expr);
}

void			sh_destroy_cmd(t_sh_cmd *cmd)
{
	t_sh_cmd *const	next = cmd->next;

	free(cmd);
	if (next != NULL)
		sh_destroy_cmd(next);
}
