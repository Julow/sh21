/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_time_clause.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/24 22:17:40 by juloo             #+#    #+#             */
/*   Updated: 2016/08/25 01:06:35 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

#include <stdlib.h>
#include <unistd.h>

static void		print_time(char const *prefix, uint64_t t)
{
	uint32_t const	min = t / T_MIN;
	uint32_t const	sec = t % T_MIN / T_SEC;
	uint32_t const	msec = t % T_SEC / T_MSEC;

	ft_dprintf(2, "%s\t%um%u.%-.3u%n", prefix, min, sec, msec);
}

static int		time_clause(t_sh_context *c, t_sh_cmd const *cmd)
{
	uint64_t		real_time;
	pid_t			pid;
	struct rusage	usage;
	int				status;

	if ((pid = fork()) < 0)
		return (ASSERT(!"fork fail"), -1);
	if (pid == 0)
	{
		g_sh_exec_cmd[cmd->type](c, cmd, true);
		HARD_ASSERT(false);
	}
	real_time = ft_clock(0);
	status = sh_wait_pid(c, pid, &usage);
	real_time = ft_clock(real_time);
	print_time("real", real_time);
	print_time("user", TIMEVAL_TO_USEC(usage.ru_utime));
	print_time("sys", TIMEVAL_TO_USEC(usage.ru_stime));
	return (status);
}

int				sh_exec_cmd_time_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	status = time_clause(c, cmd->rec);
	if (no_fork)
		exit(status);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	return (status);
}
