/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_for_clause.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/25 01:49:29 by juloo             #+#    #+#             */
/*   Updated: 2016/09/07 16:38:25 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <stdlib.h>

int				sh_exec_cmd_for_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	t_sh_exec_text	exec_text;
	t_str_list		data_buff;
	uint32_t		data_offset;
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	data_buff = STR_LIST();
	exec_text = SH_EXEC_TEXT(&cmd->for_clause->data);
	sh_exec_text(c, &exec_text, 0, &data_buff);
	status = 0;
	data_offset = 0;
	while (data_offset < STR_LIST_END(data_buff))
	{
		sh_c_var_set(c, cmd->for_clause->var,
			STR_LIST_SUB(data_buff, data_offset));
		status = sh_exec_compound(c, &cmd->for_clause->body, false);
		data_offset += STR_LIST_NEXT(data_buff, data_offset);
	}
	if (no_fork)
		exit(status);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	ft_str_list_clear(&data_buff);
	return (status);
}
