/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_subis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/09 18:28:04 by juloo             #+#    #+#             */
/*   Updated: 2016/09/09 18:33:54 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"

bool			ft_subis(t_sub sub, t_is is)
{
	char const *const	end = sub.str + sub.length;
	char const			*str;

	str = sub.str;
	while (str < end)
	{
		if (!IS(*str, is))
			return (false);
		str++;
	}
	return (true);
}

bool			ft_subis_identifier(t_sub sub)
{
	uint32_t		i;

	if (sub.length == 0 || !IS(sub.str[0], IS_ALPHA | IS_UNDERSCORE))
		return (false);
	i = 1;
	while (i < sub.length)
	{
		if (!IS(sub.str[i], IS_WORD))
			return (false);
		i++;
	}
	return (true);
}
