/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 20:32:48 by juloo             #+#    #+#             */
/*   Updated: 2016/09/05 18:36:35 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

#include <stdlib.h>
#include <unistd.h>

static int		sh_exec_cmd_empty(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	ASSERT(!"TODO: sh_exec empty");
	if (no_fork)
		exit(1);
	return (-1);
	(void)c; (void)cmd;
}

static int		sh_exec_cmd_subshell(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	pid_t			pid;
	int				status;

	if (no_fork)
	{
		sh_exec_compound(c, cmd->subshell, true);
		HARD_ASSERT(false);
	}
	if ((pid = fork()) < 0)
		return (ASSERT(!"fork fail"), -1);
	if (pid == 0)
	{
		if (!sh_exec_redir(c, &cmd->redirs, NULL))
			exit(1);
		sh_exec_compound(c, cmd->subshell, true);
		HARD_ASSERT(false);
	}
	status = sh_wait_pid(c, pid, NULL);
	return (status);
}

static int		sh_exec_cmd_not_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	status = g_sh_exec_cmd[cmd->rec->type](c, cmd->rec, false);
	status = (status == 0) ? 1 : 0;
	if (no_fork)
		exit(status);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	return (status);
}

static int		sh_exec_cmd_bracket_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	status = sh_exec_compound(c, cmd->bracket_clause, no_fork);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	return (status);
}

int				(*const g_sh_exec_cmd[])(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork) = {
	[SH_CMD_EMPTY] = &sh_exec_cmd_empty,
	[SH_CMD_SIMPLE] = &sh_exec_cmd_simple,
	[SH_CMD_SUBSHELL] = &sh_exec_cmd_subshell,
	[SH_CMD_IF_CLAUSE] = &sh_exec_cmd_if_clause,
	[SH_CMD_FOR_CLAUSE] = &sh_exec_cmd_for_clause,
	[SH_CMD_WHILE_CLAUSE] = &sh_exec_cmd_while_clause,
	[SH_CMD_UNTIL_CLAUSE] = &sh_exec_cmd_until_clause,
	[SH_CMD_TIME_CLAUSE] = &sh_exec_cmd_time_clause,
	[SH_CMD_NOT_CLAUSE] = &sh_exec_cmd_not_clause,
	[SH_CMD_BRACKET_CLAUSE] = &sh_exec_cmd_bracket_clause,
};
