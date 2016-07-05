/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:51:34 by juloo             #+#    #+#             */
/*   Updated: 2016/07/05 21:00:45 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/exec.h"

static void		build_args(t_sh_context *context, t_sh_text const *text, t_sh_args *args)
{
	t_sh_token const *const	tokens = text->tokens.data;
	uint32_t			i;
	uint32_t			text_i;
	t_vec2u				tmp;

	i = 0;
	text_i = 0;
	tmp = VEC2U(0, 0);
	while (i < text->tokens.length && tokens[i].type == SH_T_SPACE)
		i++;
	while (i < text->tokens.length)
	{
		tmp.y = 0;
		while (i < text->tokens.length && tokens[i].type != SH_T_SPACE)
		{
			switch (tokens[i].type)
			{
			case SH_T_STRING_QUOTED:
				tmp.y = 1;
			case SH_T_STRING:
				ft_dstradd(&args->buff, SUB(text->text.str + text_i, tokens[i].val.token_len));
				text_i += tokens[i].val.token_len;
				break ;
			case SH_T_REDIR:
				break ;
			case SH_T_SUBSHELL:
				break ;
			case SH_T_PARAM:
				text_i += tokens[i].val.token_len;
				break ;
			case SH_T_EXPR:
				break ;
			}
			i++;
		}
		ft_dstradd(&args->buff, SUBC("\0"));
		ft_vpush(&args->args, &tmp, 1);
		tmp.x = args->buff.length;
		while (i < text->tokens.length && tokens[i].type == SH_T_SPACE)
			i++;
	}
}

static void		destroy_args(t_sh_args *args)
{
	ft_dstrclear(&args->buff);
	ft_vclear(&args->args);
}

static int		exec_cmd(t_sh_context *context, t_sh_cmd const *cmd)
{
	t_sh_args		args;

	args = (t_sh_args){DSTR0(), VECTOR(t_vec2u)};
	build_args(context, &cmd->text, &args);
	{
		uint32_t	i;
		t_vec2u		*tmp;

		i = 0;
		while (i < args.args.length)
		{
			tmp = VECTOR_GET(args.args, i);
			ft_printf("ARG#%u '%s' %u%n", i, args.buff.str + tmp->x, tmp->y);
			i++;
		}
	}
	destroy_args(&args);
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
