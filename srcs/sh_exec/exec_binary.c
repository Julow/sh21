/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 17:59:18 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/13 15:36:37 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_dstr.h"
#include "ft/ft_printf.h"
#include "ft/ft_vector.h"

#include "p_sh_exec.h"

#include <errno.h>
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
	*args = NULL;
}

static char		*get_binary_name(t_sh_context const *c, t_sub arg0)
{
	t_dstr			buff;
	t_sub const		path = sh_var_get(c, SUBC("PATH"));
	t_sub			sub;

	buff = DSTR0();
	sub = SUB_START(path);
	while (ft_subnext_c(path, &sub, ':'))
	{
		buff.length = 0;
		ft_dstradd(&buff, sub);
		DSTR_APPEND(&buff, '/');
		ft_dstradd(&buff, arg0);
		if (access(buff.str, X_OK) == 0)
			return (buff.str);
		if (errno != ENOENT)
			ft_dprintf(2, "%ts: %s%n", DSTR_SUB(buff), strerror(errno));
	}
	ft_dstrclear(&buff);
	return (NULL);
}

static void		apply_redirs(t_vector const *redirs)
{
	t_sh_exec_redir const	*redir;

	redir = VECTOR_IT(*redirs);
	while (VECTOR_NEXT(*redirs, redir))
	{
		if (redir->flags & SH_REDIR_F_CLOSE)
			close(redir->fd.x);
		else
			dup2(redir->fd.y, redir->fd.x);
	}
}

static int		exec_binary_run(t_sh_context *context, t_sh_exec const *exec)
{
	char const		*args[exec->args.length + 1];
	char const		*env[SH_ENV_SIZE(*context) + 1];
	char			*binary;

	args_build(exec, args);
	sh_env_build(context, env);
	apply_redirs(&exec->redirs);

	{
		uint32_t		i;

		i = 0;
		while (i < exec->args.length)
		{
			ft_printf("ARG#%u '%s'%n", i, args[i]);
			i++;
		}
	}

	if ((binary = get_binary_name(context, ft_sub(args[0], 0, -1))) == NULL)
	{
		ft_dprintf(2, "%s: command not found%n", args[0]);
		return (127);
	}
	// FUCK EXECVE
	execve(binary, (char**)args, (char**)env);
	return (-1);
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
