/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:02:13 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:14:24 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

#include <stdlib.h>

static uint32_t	get_status(t_sh_context *c, t_argv const *args)
{
	t_sub			arg;
	uint32_t		s;

	if (args->argc <= 1)
		return (c->last_status);
	if (args->argc > 2)
	{
		ft_dprintf(2, "exit: too many arguments%n");
		return (1);
	}
	arg = ft_sub(args->argv[1], 0, -1);
	if (ft_subto_uint(arg, &s) != arg.length)
	{
		ft_dprintf(2, "exit: %ts: numeric argument required%n", arg);
		return (255);
	}
	return (s);
}

int				sh_builtin_exit(t_sh_context *c, void *data, t_argv args)
{
	exit(get_status(c, &args));
	return (-1);
	(void)data;
}
