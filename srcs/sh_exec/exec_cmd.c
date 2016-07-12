/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:51:34 by juloo             #+#    #+#             */
/*   Updated: 2016/07/12 13:59:40 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/exec.h"
#include "p_sh_exec.h"

static void		update_status(t_sh_context *context, int status)
{
	char			buff[16];
	uint32_t		len;

	len = ft_sprintf(buff, "%d", status);
	sh_var_set(context, SUBC("?"), SUB(buff, len));
}

int				sh_exec_cmd(t_sh_context *context, t_sh_cmd const *cmd)
{
	int				status;
	t_sh_exec		exec;

	status = -1;
	while (cmd != NULL)
	{
		exec = (t_sh_exec){DSTR0(), VECTOR(t_vec2u), VECTOR(t_sh_exec_redir)};
		build_sh_exec(context, &cmd->text, &exec);
		status = exec_binary(context, &exec);
		update_status(context, status);
		destroy_sh_exec(&exec);
		while (cmd != NULL)
		{
			while (cmd->next_type == SH_NEXT_PIPE)
				cmd = cmd->next;
			if (((status == 0) ? SH_NEXT_AND : SH_NEXT_OR) == cmd->next_type
				|| cmd->next_type == SH_NEXT_SEQ)
			{
				cmd = cmd->next;
				break ;
			}
			cmd = cmd->next;
		}
	}
	return (status);
}
