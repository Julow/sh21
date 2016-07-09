/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_sh_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 15:38:16 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/09 19:11:22 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

static void		never_called(void)
{
	HARD_ASSERT(!"never_called called");
}

static uint32_t	build_args_string(t_sh_context *context, t_sh_exec *exec,
					t_sub text, t_sh_token const *token)
{
	uint32_t const	len = token->val.token_len;

	ft_dstradd(&exec->buff, SUB_LEN(text, len));
	return (len);
	(void)context;
}

static uint32_t	build_args_redir(t_sh_context *context, t_sh_exec *exec,
					t_sub text, t_sh_token const *token)
{
	return (0);
	(void)context;
	(void)exec;
	(void)text;
	(void)token;
}

static uint32_t	build_args_subshell(t_sh_context *context, t_sh_exec *exec,
					t_sub text, t_sh_token const *token)
{
	return (0);
	(void)context;
	(void)exec;
	(void)text;
	(void)token;
}

static uint32_t	build_args_param(t_sh_context *context, t_sh_exec *exec,
					t_sub text, t_sh_token const *token)
{
	t_sub const		param = SUB_LEN(text, token->val.token_len);

	// TODO: build_args_param
	return (param.length);
	(void)context;
	(void)exec;
}

static uint32_t	build_args_expr(t_sh_context *context, t_sh_exec *exec,
					t_sub text, t_sh_token const *token)
{
	return (0);
	(void)context;
	(void)exec;
	(void)text;
	(void)token;
}

static uint32_t	(*const g_build_args[])(t_sh_context *, t_sh_exec *, t_sub,
											t_sh_token const *) =
{
	[SH_T_STRING] = &build_args_string,
	[SH_T_SPACE] = V(&never_called),
	[SH_T_REDIR] = &build_args_redir,
	[SH_T_SUBSHELL] = &build_args_subshell,
	[SH_T_PARAM] = &build_args_param,
	[SH_T_EXPR] = &build_args_expr,
};

static void		push_arg(t_sh_exec *exec, t_sh_exec_arg *arg)
{
	if (arg->offset >= exec->buff.length && !arg->quoted)
		return ;
	DSTR_APPEND(&exec->buff, '\0');
	ft_vpush(&exec->args, arg, 1);
	*arg = SH_EXEC_ARG(exec);
}

// TODO: tests: (split_arg)
// ""$TEST
// $TEST""
// $TEST
//  $TEST
// "a"$TEST
// "a"$TEST"b"
// TEST=""
// TEST="a"
// TEST="a b"
// TEST="a  b"
// TEST=" a  b"
// TEST=" "
static void		split_arg(t_sh_exec *exec, t_sh_exec_arg *arg)
{
	uint32_t		i;

	i = arg->offset;
	while (i < exec->buff.length)
	{
		if (IS(exec->buff.str[i], IS_BLANK) && (i > arg->offset || arg->quoted))
		{
			exec->buff.str[i] = '\0';
			ft_vpush(&exec->args, arg, 1);
			*arg = (t_sh_exec_arg){i + 1, false};
		}
		i++;
	}
}

void			build_sh_exec(t_sh_context *context, t_sh_text const *text,
					t_sh_exec *exec)
{
	t_sh_token const	*token;
	t_sh_exec_arg		arg;
	uint32_t			text_i;

	text_i = 0;
	arg = SH_EXEC_ARG(exec);
	token = VECTOR_IT(text->tokens);
	while (VECTOR_NEXT(text->tokens, token))
	{
		if (token->type == SH_T_SPACE)
		{
			push_arg(exec, &arg);
			continue ;
		}
		text_i += g_build_args[token->type & ~SH_F_T_QUOTED](context, exec,
				SUB_FOR(DSTR_SUB(text->text), text_i), token);
		if (token->type & SH_F_T_QUOTED)
			arg.quoted = true;
		else
			split_arg(exec, &arg);
	}
	push_arg(exec, &arg);
}

void			destroy_sh_exec(t_sh_exec *exec)
{
	ft_dstrclear(&exec->buff);
	ft_vclear(&exec->args);
	ft_vclear(&exec->redirs);
}
