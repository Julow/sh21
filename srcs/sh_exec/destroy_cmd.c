/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/14 12:42:55 by juloo             #+#    #+#             */
/*   Updated: 2016/02/14 12:54:31 by juloo            ###   ########.fr       */
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
		// else if (t->type == SH_T_EXPR)
		// 	sh_destroy_expr(t->val.expr);
		// else if (t->type == SH_T_REDIR)
		// 	sh_destroy_redir(t->val.redir);
	}
	ft_vclear(&text->tokens);
}

void			sh_destroy_cmd(t_sh_cmd *cmd)
{
	t_sh_cmd *const	next = cmd->next;

	free(cmd);
	if (next != NULL)
		sh_destroy_cmd(next);
}
