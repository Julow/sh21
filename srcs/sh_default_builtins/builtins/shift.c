/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 18:47:39 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:58:01 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

static void		ft_str_list_shift(t_str_list *lst, uint32_t shift)
{
	uint32_t		offset;

	lst->count -= shift;
	offset = 0;
	while (shift-- > 0)
		offset += STR_LIST_NEXT(*lst, offset);
	lst->last_offset -= offset;
	ft_dstrspan(&lst->buff, 0, offset, 0);
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
	if (shift >= c->pos_params.count)
		ft_str_list_clear(&c->pos_params);
	else
		ft_str_list_shift(&c->pos_params, shift);
	return (0);
	(void)data;
}
