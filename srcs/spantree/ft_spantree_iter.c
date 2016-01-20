/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spantree_iter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 16:10:33 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/20 16:13:49 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spantree.h"

static void		spantree_iter(t_spantree_node const *parent, t_vec2i range,
					t_callback *callback, t_spantree_data *prev)
{
	t_spantree_node const	*span;
	t_spantree_data			data;

	data.prev = prev;
	span = parent->childs;
	while (span != NULL)
	{
		if (span->to > range.x)
		{
			if (span->from > range.y)
				break ;
			if (span->from > range.x)
				CALL(void, *callback, range.x, span->from, prev);
			data.data = span->data;
			range.x = span->from;
			if (span->to > range.x)
				spantree_iter(span, range, callback, &data);
			range.x = span->to;
		}
		span = span->next;
	}
	if (range.x < parent->to && range.x < range.y)
		CALL(void, *callback, range.x, MIN(parent->to, range.y), prev);
}

void			ft_spantree_iter(t_spantree const *tree, t_vec2i range,
					t_callback callback)
{
	t_spantree_node		span;

	span = (t_spantree_node){range.x, range.y, NULL, tree->spans, NULL};
	spantree_iter(&span, range, &callback, NULL);
}
