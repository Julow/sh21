/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:24:26 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/09 18:32:59 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "ft/utils.h"

#include "p_default_builtins.h"

int				sh_builtin_unset(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;
	int				status;

	while (ft_argv_opt(&args, &arg))
	{
		ft_dprintf(2, "unset: %ts: Unknown option%n", arg);
		return (1);
	}
	status = 0;
	while (ft_argv_arg(&args, &arg))
	{
		if (!ft_subis_identifier(arg))
		{
			status = 1;
			ft_dprintf(2, "unset: %ts: not a valid identifier%n", arg);
		}
		else
			sh_c_var_unset(c, arg);
	}
	return (status);
	(void)data;
}
