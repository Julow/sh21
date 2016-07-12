/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_sh_exec_string.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/12 14:43:39 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/12 15:02:13 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

bool			build_args_string(t_sh_exec_builder *b, t_sh_token const *t)
{
	uint32_t const	len = t->val.token_len;

	ft_dstradd(&b->dst->buff, SUB(b->text->text.str + b->text_i, len));
	b->text_i += len;
	return (true);
}

bool			build_args_param(t_sh_exec_builder *b, t_sh_token const *t)
{
	t_sub const		param = SUB(b->text->text.str + b->text_i, t->val.token_len);

	ft_dstradd(&b->dst->buff, sh_var_get(b->context, param));
	b->text_i += param.length;
	return (true);
}

bool			build_args_space(t_sh_exec_builder *b, t_sh_token const *t)
{
	build_sh_exec_push(b);
	return (true);
	(void)t;
}
