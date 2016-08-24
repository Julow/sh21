/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_for_clause.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/25 01:49:29 by juloo             #+#    #+#             */
/*   Updated: 2016/08/25 01:49:51 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <stdlib.h>

int				sh_exec_cmd_for_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	t_dstr			data_buff;
	uint32_t		data_count;
	t_vec2u			data_sub;
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	data_buff = DSTR0();
	data_count = sh_exec_text(c, &cmd->for_clause->data, &data_buff);
	status = 0;
	data_sub = VEC2U(0, 0);
	while (data_count-- > 0)
	{
		while (data_buff.str[data_sub.y] != '\0')
			data_sub.y++;
		sh_var_set(c, cmd->for_clause->var,
			SUB(data_buff.str + data_sub.x, data_sub.y - data_sub.x));
		status = sh_exec_compound(c, &cmd->for_clause->body, false);
		data_sub = VEC2U1(data_sub.y + 1);
	}
	if (no_fork)
		exit(status);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	ft_dstrclear(&data_buff);
	return (status);
}
