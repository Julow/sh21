/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/05 18:34:01 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/06 22:17:22 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_viewport.h"

static void		notify_listeners(t_editor_viewport *v,
					t_editor_viewport_event const *event)
{
	t_editor_viewport_listener	**l;

	l = VECTOR_IT(v->listeners);
	while (VECTOR_NEXT(v->listeners, l))
		(*l)->on_change(*l, event);
}

static uint32_t	line_height(t_editor_viewport const *v, uint32_t i)
{
	uint32_t const	len = EDITOR_LINE(v->editor, i + v->scroll.y);

	return ((MAX(len, 1) + v->size.x - 1) / v->size.x);
}

static uint32_t	next_change(t_editor_viewport const *v, uint32_t *i,
					uint32_t total_height, uint32_t *height)
{
	while (i < v->line_heights.length)
	{
		*height = line_heights(v, *i);
		if (*height != VGETC(uint32_t, v->line_heights, *i))
			return (total_height);
		total_height += *height;
		(*i)++;
	}
	if (total_height < v->size.y)
		*height = line_heights(v, *i);
	return (total_height);
}

void			editor_update_line_heights(t_editor_viewport *v, bool event)
{
	uint32_t		old_heights[v->line_heights.length];
	uint32_t		i;
	uint32_t		total_height;
	uint32_t		tmp;

	i = 0;
	if ((total_height = next_change(v, &i, 0, &tmp)) >= v->size.y)
		return ;
	memcpy(old_heights, v->line_heights.data, sizeof(old_heights));
	while (true)
	{
		if (i >= v->line_heights.length)
			ft_vpush(&v->line_heights, NULL, 1);
		VGET(uint32_t, v->line_heights, i) = tmp;
		i++;
		total_height = next_change(v, &i, total_height + tmp, &tmp);
		if (total_height >= v->size.y)
			break ;
	}
	v->line_heights.length = i;
	if (event)
		notify_listeners(v, &(t_editor_viewport_event){
			EDITOR_VIEWPORT_EVENT_LINE_HEIGHTS,
			{ .old_heights = VECTORC(old_heights) }
		});
}
