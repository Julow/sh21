/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:40:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/10 23:08:53 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

void			ft_str_list_trunc(t_str_list *lst, uint32_t i) // TODO: move
{
	uint32_t		offset;

	if (i == 0)
	{
		ft_str_list_clear(lst);
		return ;
	}
	offset = 0;
	lst->count = i;
	while (i-- > 1)
		offset += STR_LIST_NEXT(*lst, offset);
	lst->buff.length = offset + STR_LIST_NEXT(*lst, offset);
	lst->last_break = true;
	lst->last_offset = offset;
}

int				sh_builtin_set(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;

	while (ft_argv_opt(&args, &arg))
	{
		ft_dprintf(2, "set: %ts: Unknown option%n", arg);
		return (1);
	}
	ft_str_list_trunc(&c->pos_params, 1);
	while (ft_argv_arg(&args, &arg))
	{
		ft_str_list_append(&c->pos_params, arg);
		ft_str_list_break(&c->pos_params);
	}
	return (0);
	(void)data;
}
