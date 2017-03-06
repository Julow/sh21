/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/05 15:59:30 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/06 22:16:41 by jaguillo         ###   ########.fr       */
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

void			editor_set_size(t_editor_viewport *v, t_vec2u size)
{
	t_vec2u const	old_size = v->size;

	if (old_size.x == size.x && old_size.y == size.y)
		return ;
	editor_update_line_heights(v, false);
	notify_listeners(v, &(t_editor_viewport_event){
		EDITOR_VIEWPORT_EVENT_SIZE,
		{ .old_size = old_size }
	});
}

void			editor_set_scroll(t_editor_viewport *v, t_vec2u scroll)
{
	t_vec2u const	old_scroll = v->scroll;

	if (old_scroll.y != scroll.y)
		editor_update_line_heights(v, false);
	else if (old_scroll.x == scroll.x)
		return ;
	notify_listeners(v, &(t_editor_viewport_event){
		EDITOR_VIEWPORT_EVENT_SCROLL,
		{ .old_scroll = old_scroll }
	});
}

void			editor_set_linewrap(t_editor_viewport *v, bool enabled)
{
	if (enabled == (bool)(v->flags & EDITOR_LINE_WRAP))
		return ;
	editor_update_line_heights(v, false);
	notify_listeners(v, &(t_editor_viewport_event){
		EDITOR_VIEWPORT_EVENT_LINE_WRAP, {}
	});
}
