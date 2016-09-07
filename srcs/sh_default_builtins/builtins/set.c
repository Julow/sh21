/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:40:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:46:50 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

int				sh_builtin_set(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;

	while (ft_argv_opt(&args, &arg))
	{
		ft_dprintf(2, "set: %ts: Unknown option%n", arg);
		return (1);
	}
	ft_str_list_clear(&c->pos_params);
	while (ft_argv_arg(&args, &arg))
	{
		ft_str_list_append(&c->pos_params, arg);
		ft_str_list_break(&c->pos_params);
	}
	return (0);
	(void)data;
}
