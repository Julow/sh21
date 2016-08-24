/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 20:53:56 by juloo             #+#    #+#             */
/*   Updated: 2016/08/24 22:07:55 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_sh_exec.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void		sh_exec_text_load_params(t_dstr const *param_buff,
					uint32_t param_count, char const **dst)
{
	uint32_t		param_i;
	uint32_t		buff_i;

	param_i = 0;
	buff_i = 0;
	while (param_i < param_count)
	{
		ft_printf("Param#%u '%s'%n", param_i, param_buff->str + buff_i);
		dst[param_i++] = param_buff->str + buff_i;
		while (param_buff->str[buff_i++] != '\0')
			;
	}
	dst[param_i] = NULL;
}

__attribute__ ((noreturn))
static void		sh_exec_cmd_simple_exec(t_sh_context *c, t_sh_cmd const *cmd,
					t_dstr const *param_buff, uint32_t param_count)
{
	char const		*params[param_count + 1];
	t_sub			cmd_name;
	t_dstr			cmd_path;

	sh_exec_text_load_params(param_buff, param_count, params);
	cmd_name = ft_sub(params[0], 0, -1);
	if (!sh_exec_redir(c, &cmd->redirs, NULL))
		exit(1);
	cmd_path = DSTR0();
	if (ft_subfind_c(cmd_name, '/', 0) < cmd_name.length)
		ft_dstradd(&cmd_path, cmd_name);
	else if (!sh_search_path(c, cmd_name, &cmd_path))
	{
		ft_dprintf(2, "%ts: command not found%n", cmd_name);
		exit(127);
	}
	execve(cmd_path.str, V(params), NULL);
	ft_printf("%ts: %s%n", DSTR_SUB(cmd_path), strerror(errno));
	exit(126);
}

static int		sh_exec_cmd_simple_fork(t_sh_context *c, t_sh_cmd const *cmd,
					t_dstr const *param_buff, uint32_t param_count)
{
	pid_t			pid;

	if ((pid = fork()) < 0)
		return (ASSERT(!"fork fail"), -1);
	if (pid == 0)
	{
		sh_exec_cmd_simple_exec(c, cmd, param_buff, param_count);
		ASSERT(false);
		exit(-1);
	}
	return (sh_wait_pid(c, pid, NULL));
}

int				sh_exec_cmd_simple(t_sh_context *c, t_sh_cmd const *cmd,
					bool no_fork)
{
	t_dstr			param_buff;
	uint32_t		param_count;
	int				status;

	param_buff = DSTR0();
	param_count = sh_exec_text(c, &cmd->simple.text, &param_buff);
	// TODO: builtins
	if (no_fork)
		sh_exec_cmd_simple_exec(c, cmd, &param_buff, param_count);
	else
		status = sh_exec_cmd_simple_fork(c, cmd, &param_buff, param_count);
	ft_dstrclear(&param_buff);
	return (status);
}
