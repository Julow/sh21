/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/22 14:55:41 by juloo             #+#    #+#             */
/*   Updated: 2016/08/24 21:41:47 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

bool			sh_search_path(t_sh_context const *c, t_sub name, t_dstr *dst)
{
	t_sub const		path = sh_var_get(c, SUBC("PATH"));
	t_sub			sub;

	sub = SUB_START(path);
	while (ft_subnext_c(path, &sub, ':'))
	{
		if (sub.length == 0)
			continue ;
		dst->length = 0;
		ft_dstradd(dst, sub);
		if (dst->str[dst->length - 1] != '/')
			ft_dstradd(dst, SUBC("/"));
		ft_dstradd(dst, name);
		if (access(dst->str, X_OK) == 0)
			return (true);
	}
	return (false);
}

int				sh_wait_pid(t_sh_context *c, pid_t pid, struct rusage *u)
{
	int				status;

	wait4(pid, &status, WUNTRACED, u);
	if (WIFSTOPPED(status))
		return (TRACE("Stopped"), 0);
	if (WIFSIGNALED(status))
		return (TRACE("Killed"), WTERMSIG(status) + 128);
	ASSERT(WIFEXITED(status));
	return (WEXITSTATUS(status));
	(void)c;
}
