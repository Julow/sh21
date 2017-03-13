/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 18:22:37 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/12 02:07:17 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_viewport.h"
#include "p_editor_viewport.h"

#include <stddef.h>

static void		on_text_change(t_editor_viewport *v,
					t_editor_text_event const *event, bool batch)
{
	v = V(v) - offsetof(t_editor_viewport, text_listener);
	if (event->begin.x < v->scroll.y)
		v->scroll.y += event->line_count - MIN(v->scroll.y, event->end.x)
				+ event->begin.x;
	if (v->flags & EDITOR_LINE_WRAP)
		editor_update_line_heights(v, true);
	(void)batch;
}

static void		on_cursor_change(t_editor_viewport *v,
					t_editor_sel const *new_cursors, uint32_t len, bool batch)
{
	v = V(v) - offsetof(t_editor_viewport, cursor_listener);
	// TODO: scroll
	(void)new_cursors;
	(void)len;
	(void)batch;
}

void			editor_viewport_init(t_editor *e, t_editor_viewport *dst)
{
	*dst = (t_editor_viewport){
		e,
		VEC2U(0, 0),
		VEC2U(0, 0),
		VECTOR(uint32_t),
		0,
		VECTOR(t_editor_viewport_listener),
		EDITOR_TEXT_LISTENER(on_text_change),
		EDITOR_CURSOR_LISTENER(on_cursor_change),
	};
	editor_register_listener(e, &dst->text_listener, &dst->cursor_listener);
}
