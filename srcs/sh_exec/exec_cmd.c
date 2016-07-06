/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:51:34 by juloo             #+#    #+#             */
/*   Updated: 2016/07/06 16:48:14 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/exec.h"
#include "p_sh_exec.h"

static int		exec_cmd(t_sh_context *context, t_sh_cmd const *cmd)
{
	t_sh_exec		exec;

	exec = (t_sh_exec){DSTR0(), VECTOR(t_vec2u), VECTOR(t_sh_exec_redir)};
	build_sh_exec(context, &cmd->text, &exec);
	{
		uint32_t		i;

		ft_printf("EXEC [%n");
		i = 0;
		while (i < exec.args.length)
		{
			t_sh_exec_arg	*arg = VECTOR_GET(exec.args, i);
			ft_printf("\tARG#%u '%s' %u%n", i, exec.buff.str + arg->offset, arg->quoted);
			i++;
		}

		i = 0;
		while (i < exec.redirs.length)
		{
			t_sh_exec_redir	*redir = VECTOR_GET(exec.redirs, i);
			ft_printf("\tREDIR#%u %u%n", i, *redir);
			i++;
		}
		ft_printf("]%n");
	}
	destroy_sh_exec(&exec);
	return (0);
}

int				sh_exec_cmd(t_sh_context *context, t_sh_cmd const *cmd)
{
	int				status;

	status = -1;
	while (cmd != NULL)
	{
		status = exec_cmd(context, cmd);
		while (cmd != NULL)
		{
			while (cmd->next_type == SH_NEXT_PIPE)
				cmd = cmd->next;
			if (((status == 0) ? SH_NEXT_AND : SH_NEXT_OR) == cmd->next_type
				|| cmd->next_type == SH_NEXT_NEW)
			{
				cmd = cmd->next;
				break ;
			}
			cmd = cmd->next;
		}
	}
	return (status);
}
