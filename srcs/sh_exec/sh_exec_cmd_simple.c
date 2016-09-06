/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 20:53:56 by juloo             #+#    #+#             */
/*   Updated: 2016/09/06 18:08:04 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_sh_exec.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void		sh_exec_text_load_params(t_str_list const *param_buff,
					char const **dst)
{
	uint32_t		param_offset;
	uint32_t		dst_i;

	param_offset = 0;
	dst_i = 0;
	while (param_offset < STR_LIST_END(*param_buff))
	{
		dst[dst_i++] = STR_LIST_STR(*param_buff, param_offset);
		param_offset += STR_LIST_NEXT(*param_buff, param_offset);
	}
	dst[dst_i] = NULL;
}

__attribute__ ((noreturn))
static void		sh_exec_cmd_simple_exec(t_sh_context *c, t_sh_cmd const *cmd,
					t_str_list const *param_buff)
{
	char const		*params[param_buff->count + 1];
	t_sub			cmd_name;
	t_dstr			cmd_path;

	sh_exec_text_load_params(param_buff, params);
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
					t_str_list const *param_buff)
{
	pid_t			pid;

	if ((pid = fork()) < 0)
		return (ASSERT(!"fork fail"), -1);
	if (pid == 0)
	{
		sh_exec_cmd_simple_exec(c, cmd, param_buff);
		ASSERT(false);
		exit(-1);
	}
	return (sh_wait_pid(c, pid, NULL));
}

int				sh_exec_cmd_simple(t_sh_context *c, t_sh_cmd const *cmd,
					bool no_fork)
{
	t_str_list		param_buff;
	t_sh_exec_text	exec_text;
	int				status;

	param_buff = STR_LIST();
	exec_text = SH_EXEC_TEXT(&cmd->simple.text);
	sh_exec_text(c, &exec_text, 0, &param_buff);
	// TODO: builtins
	if (no_fork)
		sh_exec_cmd_simple_exec(c, cmd, &param_buff);
	else
		status = sh_exec_cmd_simple_fork(c, cmd, &param_buff);
	ft_str_list_clear(&param_buff);
	return (status);
}
