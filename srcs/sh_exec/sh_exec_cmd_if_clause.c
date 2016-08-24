/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_if_clause.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/24 20:10:03 by juloo             #+#    #+#             */
/*   Updated: 2016/08/24 20:22:07 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

static int		exec_if(t_sh_context *c,
					t_sh_if const *if_clause, bool no_fork)
{
	int const		status = sh_exec_compound(c, &if_clause->cond, false);

	if (status == 0)
		return (sh_exec_compound(c, &if_clause->body, no_fork));
	else if (if_clause->else_clause != NULL)
		return ((if_clause->else_clause->type == SH_ELSE_ELIF) ?
			exec_if(c, &if_clause->else_clause->elif_clause, no_fork) :
			sh_exec_compound(c, &if_clause->else_clause->else_clause, no_fork));
	else
		return (0);
}

int				sh_exec_cmd_if_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	status = exec_if(c, cmd->if_clause, no_fork);
	HARD_ASSERT(!no_fork);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	return (status);
}
