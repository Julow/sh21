/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 15:19:44 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/14 16:08:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/range.h"

t_range		ft_range(int32_t from, int32_t to)
{
	return (SRANGE(from, to));
}

t_range		ft_rangeclip(t_range range, t_range clip)
{
	if (range.from > range.to)
		range = RANGE(range.to, range.from);
	if (range.to > clip.to)
	{
		range.to = clip.to;
		if (range.to < range.from)
			range.from = range.to;
	}
	if (range.from < clip.from)
	{
		range.from = clip.from;
		if (range.to < range.from)
			range.to = range.from;
	}
	return (range);
}

t_range		ft_subrange(t_sub a, t_sub b)
{
	int32_t		from;

	from = a.str - b.str;
	if (from < 0 || from > a.length)
		return (RANGE(0, 0));
	if ((from + b.length) > a.length)
		return (RANGE(a.length, a.length));
	return (RANGE(from, from + b.length));
}
