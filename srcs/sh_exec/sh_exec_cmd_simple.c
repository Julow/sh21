/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 20:53:56 by juloo             #+#    #+#             */
/*   Updated: 2016/09/10 16:20:27 by jaguillo         ###   ########.fr       */
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
	char const		*env[SH_C_ENV_SIZE(*c)];
	t_sub			cmd_name;
	t_dstr			cmd_path;

	cmd_name = STR_LIST_SUB(*param_buff, 0);
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
	sh_exec_text_load_params(param_buff, params);
	sh_c_env_build(c, env);
	execve(cmd_path.str, V(params), V(env));
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

void			ft_str_list_copy(t_str_list const *src, t_str_list *dst) // TODO: move
{
	dst->buff = DSTR0();
	ft_dstradd(&dst->buff, DSTR_SUB(src->buff));
	dst->last_break = src->last_break;
	dst->last_offset = src->last_offset;
	dst->count = src->count;
}

static bool		sh_exec_cmd_simple_function(t_sh_context *c,
					t_str_list const *param_buff, bool no_fork, int *status)
{
	t_sh_cmd const		*cmd;
	t_str_list			save_params;

	if ((cmd = sh_c_function_get(c, STR_LIST_SUB(*param_buff, 0))) == NULL)
		return (false);
	save_params = c->pos_params;
	ft_str_list_copy(param_buff, &c->pos_params);
	*status = g_sh_exec_cmd[cmd->type](c, cmd, no_fork);
	ft_str_list_clear(&c->pos_params);
	c->pos_params = save_params;
	return (true);
}

static bool		sh_exec_cmd_simple_builtin(t_sh_context *c,
					t_str_list const *param_buff, bool no_fork, int *status)
{
	char const		*params[param_buff->count + 1];
	t_sh_builtin	builtin;
	void			*builtin_data;

	if (!sh_c_builtin_get(c, STR_LIST_SUB(*param_buff, 0),
			&builtin, &builtin_data))
		return (false);
	sh_exec_text_load_params(param_buff, params);
	*status = builtin(c, builtin_data, ARGV(param_buff->count, V(params)));
	if (no_fork)
		exit(*status);
	return (true);
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
	HARD_ASSERT(param_buff.count > 0);
	if (sh_exec_cmd_simple_function(c, &param_buff, no_fork, &status))
		;
	else if (sh_exec_cmd_simple_builtin(c, &param_buff, no_fork, &status))
		;
	else if (no_fork)
		sh_exec_cmd_simple_exec(c, cmd, &param_buff);
	else
		status = sh_exec_cmd_simple_fork(c, cmd, &param_buff);
	ft_str_list_clear(&param_buff);
	return (status);
}
