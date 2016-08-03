/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_compound.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 17:19:52 by juloo             #+#    #+#             */
/*   Updated: 2016/07/30 14:46:34 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/ast.h"
#include "p_destroy_cmd.h"

#include <stdlib.h>

static void		sh_destroy_cmd(t_sh_cmd *cmd)
{
	switch (cmd->type)
	{
	case SH_CMD_SIMPLE:
		sh_destroy_text(&cmd->simple.text);
		break ;
	case SH_CMD_SUBSHELL:
		sh_destroy_text(&cmd->subshell->text);
		sh_destroy_compound(&cmd->subshell->compound);
		free(cmd->subshell);
		break ;
	case SH_CMD_IF_CLAUSE:
		sh_destroy_if_clause(cmd->if_clause);
		free(cmd->if_clause);
		break ;
	case SH_CMD_FOR_CLAUSE:
		sh_destroy_for_clause(cmd->for_clause);
		free(cmd->for_clause);
		break ;
	case SH_CMD_WHILE_CLAUSE:
	case SH_CMD_UNTIL_CLAUSE:
		sh_destroy_while_clause(cmd->while_clause);
		free(cmd->while_clause);
		break ;
	}
}

static void		sh_destroy_pipeline(t_sh_pipeline *p)
{
	sh_destroy_cmd(&p->cmd);
	if (p->next != NULL)
	{
		sh_destroy_pipeline(p->next);
		free(p->next);
	}
}

static void		sh_destroy_list(t_sh_list *list)
{
	sh_destroy_pipeline(&list->pipeline);
	if (list->next != NULL)
	{
		sh_destroy_list(&list->next->next);
		free(list->next);
	}
}

void			sh_destroy_compound(t_sh_compound *c)
{
	sh_destroy_list(&c->list);
	if (c->next != NULL)
	{
		sh_destroy_compound(c->next);
		free(c->next);
	}
}