/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_sh_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 15:38:16 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/12 15:04:38 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

static bool		(*const g_build_args[])(t_sh_exec_builder *, t_sh_token const *) =
{
	[SH_T_STRING] = &build_args_string,
	[SH_T_SPACE] = &build_args_space,
	[SH_T_REDIR] = &build_args_redir,
	[SH_T_SUBSHELL] = &build_args_subshell,
	[SH_T_PARAM] = &build_args_param,
	[SH_T_EXPR] = &build_args_expr,
};

//TODO: fix: `$TEST` with TEST=" "
static void		split_arg(t_sh_exec_builder *b)
{
	uint32_t		i;

	i = b->arg.offset;
	while (i < b->dst->buff.length)
	{
		if (IS(b->dst->buff.str[i], IS_BLANK)
			&& (i >= b->arg.offset || b->arg.quoted))
		{
			b->dst->buff.str[i] = '\0';
			ft_vpush(&b->dst->args, &b->arg, 1);
			b->arg = (t_sh_exec_arg){i + 1, false};
		}
		i++;
	}
}

bool			build_sh_exec_next(t_sh_exec_builder *b)
{
	t_sh_token const *const	token = VECTOR_GET(b->text->tokens, b->token_i++);

	if (!g_build_args[token->type & ~SH_F_T_QUOTED](b, token))
		return (false);
	if (token->type & SH_F_T_QUOTED)
		b->arg.quoted = true;
	else
		split_arg(b);
	return (true);
}

void			build_sh_exec_push(t_sh_exec_builder *b)
{
	if (b->arg.offset >= b->dst->buff.length && !b->arg.quoted)
		return ;
	DSTR_APPEND(&b->dst->buff, '\0');
	ft_vpush(&b->dst->args, &b->arg, 1);
	b->arg = SH_EXEC_ARG(b->dst);
}

bool			build_sh_exec(t_sh_context *context, t_sh_text const *text,
					t_sh_exec *exec)
{
	t_sh_exec_builder	b;

	b = (t_sh_exec_builder){context, text, 0, 0, SH_EXEC_ARG(exec), exec};
	while (b.token_i < text->tokens.length)
		if (!build_sh_exec_next(&b))
		{
			destroy_sh_exec(exec);
			return (false);
		}
	build_sh_exec_push(&b);
	return (true);
}

void			destroy_sh_exec(t_sh_exec *exec)
{
	ft_dstrclear(&exec->buff);
	ft_vclear(&exec->args);
	ft_vclear(&exec->redirs);
}
