/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_compound.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/18 22:18:59 by juloo             #+#    #+#             */
/*   Updated: 2016/08/22 22:59:53 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int				sh_exec_pipeline(t_sh_context *c,
					t_sh_pipeline const *cmd, bool no_fork)
{
	int				fd_in;
	int				fd_pipe[2];
	int				saved_stdin;
	int				status;
	pid_t			pid;

	if (cmd->next == NULL)
		return (g_sh_exec_cmd[cmd->cmd.type](c, &cmd->cmd, no_fork));
	fd_in = 0;
	while (cmd->next != NULL)
	{
		if (pipe(fd_pipe) < 0)
			HARD_ASSERT(!"pipe fail");

		if ((pid = fork()) < 0)
			HARD_ASSERT(!"fork fail");
		if (pid == 0)
		{
			close(fd_pipe[0]);
			if (fd_in != 0)
				dup2(fd_in, 0);
			dup2(fd_pipe[1], 1);
			g_sh_exec_cmd[cmd->cmd.type](c, &cmd->cmd, true);
			HARD_ASSERT(false);
		}

		if (fd_in != 0)
			close(fd_in);
		fd_in = fd_pipe[0];
		close(fd_pipe[1]);
		cmd = cmd->next;
	}

	saved_stdin = no_fork ? 0 : fcntl(0, F_DUPFD_CLOEXEC, 10);
	dup2(fd_in, 0);
	status = g_sh_exec_cmd[cmd->cmd.type](c, &cmd->cmd, no_fork);
	dup2(saved_stdin, 0);
	return (status);
}

int				sh_exec_list(t_sh_context *c,
					t_sh_list const *cmd, bool no_fork)
{
	int						status;

	status = 0;
	while (true)
	{
		status = sh_exec_pipeline(c, &cmd->pipeline,
				cmd->next == NULL && no_fork);
		while (cmd->next != NULL
			&& cmd->next->type == ((status == 0) ? SH_LIST_OR : SH_LIST_AND))
			cmd = &cmd->next->next;
		if (cmd->next == NULL)
			break ;
		cmd = &cmd->next->next;
	}
	return (status);
}

int				sh_exec_list_async(t_sh_context *c, t_sh_list const *cmd)
{
	pid_t			pid;

	if ((pid = fork()) < 0)
		return (ASSERT(!"fork fail"), -1);
	if (pid == 0)
	{
		sh_exec_list(c, cmd, true);
		HARD_ASSERT(false);
	}
	return (0);
}

int				sh_exec_compound(t_sh_context *c,
					t_sh_compound const *cmd, bool no_fork)
{
	int				status;

	status = 0;
	while (cmd != NULL)
	{
		if (cmd->flags & SH_COMPOUND_ASYNC)
			status = sh_exec_list_async(c, &cmd->list);
		else
			status = sh_exec_list(c, &cmd->list, cmd->next == NULL && no_fork);
		cmd = cmd->next;
	}
	ASSERT(!no_fork);
	return (status);
}
