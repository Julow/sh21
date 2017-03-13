/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/05 18:34:01 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/12 22:45:25 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_viewport.h"
#include "p_editor_viewport.h"

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
	uint32_t		len;

	i += v->scroll.y;
	if (i >= SUMSET_LENGTH(v->editor->lines))
		return (1);
	len = EDITOR_LINE(v->editor, i)
		- (i + 1 >= SUMSET_LENGTH(v->editor->lines));
	return ((len + v->size.x - 1) / v->size.x);
}

static uint32_t	next_change(t_editor_viewport const *v, uint32_t *i,
					uint32_t total_height, uint32_t *height)
{
	while (*i < v->line_heights.length)
	{
		*height = line_height(v, *i);
		if (*height != VGETC(t_vec2u, v->line_heights, *i).x)
			return (total_height);
		total_height += *height;
		(*i)++;
	}
	if (total_height < v->size.y)
		*height = line_height(v, *i);
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
		VGET(t_vec2u, v->line_heights, i) = VEC2U(tmp, total_height);
		i++;
		total_height += tmp;
		if (total_height >= v->size.y)
			break ;
		tmp = line_height(v, i);
	}
	v->line_heights.length = i;
	if (event)
		notify_listeners(v, &(t_editor_viewport_event){
			EDITOR_VIEWPORT_EVENT_LINE_HEIGHTS,
			{ .old_heights = VECTORC(old_heights) }
		});
}
