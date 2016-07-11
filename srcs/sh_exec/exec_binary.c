/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 17:59:18 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 18:26:04 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_sh_exec.h"

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

static void		args_build(t_sh_exec const *exec, char const **args)
{
	t_sh_exec_arg const	*arg;

	arg = VECTOR_IT(exec->args);
	while (VECTOR_NEXT(exec->args, arg))
	{
		*args = exec->buff.str + arg->offset;
		args++;
	}
}

static int		exec_binary_run(t_sh_context *context, t_sh_exec const *exec)
{
	char const		*args[exec->args.length];
	char const		*env[SH_ENV_SIZE(*context)];

	args_build(exec, args);
	sh_env_build(context, env);

	TRACE("fork");
	return (127);
}

static int		wait_child(t_sh_context *context, pid_t pid)
{
	int				status;

	if (wait4(pid, &status, WUNTRACED, NULL) < 0)
		return (-1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		ft_printf("Killed by signal: %d%s%n", WTERMSIG(status),
			WCOREDUMP(status) ? " (Core dump)" : "");
		return (128 + WTERMSIG(status));
	}
	if (WIFSTOPPED(status))
	{
		ft_printf("Stopped%n");
		return (128 + WSTOPSIG(status));
	}
	HARD_ASSERT(false);
	(void)context;
	return (-1);
}

int				exec_binary(t_sh_context *context, t_sh_exec const *exec)
{
	pid_t			pid;

	if ((pid = fork()) < 0)
	{
		ft_dprintf(2, "Fork failed%n");
		return (-1);
	}
	if (pid == 0)
		exit(exec_binary_run(context, exec));
	return (wait_child(context, pid));
}
