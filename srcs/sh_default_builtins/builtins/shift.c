/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:47:39 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/10 23:07:35 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

static void		ft_str_list_shift(t_str_list *lst, uint32_t shift, uint32_t i)
{
	uint32_t		from;
	uint32_t		to;

	lst->count -= shift;
	from = 0;
	while (i-- > 0)
		from += STR_LIST_NEXT(*lst, from);
	to = from;
	while (shift-- > 0)
		to += STR_LIST_NEXT(*lst, to);
	lst->last_offset -= to - from;
	ft_dstrspan(&lst->buff, from, to, 0);
}

int				sh_builtin_shift(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;
	uint32_t		shift;

	shift = 1;
	if (args.argc > 1)
	{
		arg = ft_sub(args.argv[1], 0, -1);
		if (ft_subto_uint(arg, &shift) != arg.length)
		{
			ft_dprintf(2, "shift: %ts: numeric argument required%n", arg);
			return (1);
		}
	}
	if (args.argc > 2)
	{
		ft_dprintf(2, "shift: too many arguments%n");
		return (1);
	}
	if (shift >= c->pos_params.count - 1)
		ft_str_list_trunc(&c->pos_params, 1);
	else
		ft_str_list_shift(&c->pos_params, shift, 1);
	return (0);
	(void)data;
}
