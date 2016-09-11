/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/22 14:55:41 by juloo             #+#    #+#             */
/*   Updated: 2016/09/11 19:21:53 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_sh_exec.h"

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int				sh_wait_pid(t_sh_context *c, pid_t pid, struct rusage *u)
{
	int				status;

	wait4(pid, &status, WUNTRACED, u);
	if (WIFSTOPPED(status))
	{
		ft_printf("Stopped%n");
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		ft_printf("Killed by signal %u%n", WTERMSIG(status));
		return (WTERMSIG(status) + 128);
	}
	ASSERT(WIFEXITED(status));
	return (WEXITSTATUS(status));
	(void)c;
}
