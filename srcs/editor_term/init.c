/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 19:53:22 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/20 22:03:50 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_term.h"
#include <stddef.h>

#define _UPDATE_PUSH(U,...)	&(t_editor_term_update){EDITOR_TERM_UPDATE_##U, {__VA_ARGS__}}
#define UPDATE_PUSH(T,U,...)	ft_vpush(&(T)->updates, _UPDATE_PUSH(U,__VA_ARGS__), 1)

static void		editor_term_text_change(t_editor_term *t,
					t_editor_text_event const *e, bool batch)
{
	int32_t const	line_diff = e->line_count
							- (e->sel_end.x - e->sel_begin.x + 1);

	if (line_diff > 0)
		UPDATE_PUSH(t, INSERT_LINE, .insert_line={ e->sel_begin.x + 1, line_diff });
	else if (line_diff < 0)
		UPDATE_PUSH(t, DELETE_LINE, .delete_line={ e->sel_begin.x + 1, -line_diff });
	if (t->scroll.x + t->size.x > e->sel_begin.y) // TODO: ?
		// TODO: INSERT_CHAR/REMOVE_CHAR update
		// TODO: don't ce if not needed
		UPDATE_PUSH(t, REDRAW, .redraw={ e->sel_begin.x, e->sel_begin.y,
				EDITOR_LINE(t->editor, e->sel_begin.x) - e->sel_begin.y });
}

static void		update_cursor(t_editor_term *t, t_editor_sel const *sel)
{
	t_vec2u const	a = editor_getpos(t->editor, EDITOR_SEL_BEGIN(*sel));
	t_vec2u const	b = (sel->x == sel->y) ? a
						: editor_getpos(t->editor, EDITOR_SEL_END(*sel));
	uint32_t		line;

	if (a.x == b.x)
	{
		UPDATE_PUSH(t, REDRAW, .redraw={ a.x, a.y, b.y - a.y });
		return ;
	}
	UPDATE_PUSH(t, REDRAW, .redraw={ a.x, a.y, EDITOR_LINE(t->editor, a.x) - a.y });
	line = a.x + 1;
	while (line + 1 < b.x)
	{
		UPDATE_PUSH(t, REDRAW, .redraw={ line, 0, EDITOR_LINE(t->editor, line) });
		line++;
	}
	UPDATE_PUSH(t, REDRAW, .redraw={ b.x, 0, b.y });
}

static void		editor_term_cursor_change(t_editor_term *t,
					t_editor_sel const *new_cursors, uint32_t len, bool batch)
{
	uint32_t			i;

	t = V(t) - offsetof(t_editor_term, cursor_listener);
	i = 0;
	while (i < t->editor->cursors.length && i < len)
	{
		update_cursor(t, VECTOR_GET(t->editor->cursors, i));
		update_cursor(t, &new_cursors[i]);
		i++;
	}
	while (i < t->editor->cursors.length)
		update_cursor(t, VECTOR_GET(t->editor->cursors, i++));
	while (i < len)
		update_cursor(t, &new_cursors[i++]);
	(void)batch;
}

void			editor_term_init(t_editor_term *dst, t_term *term,
					t_editor *editor, t_vec2u size)
{
	*dst = (t_editor_term){
		EDITOR_TEXT_LISTENER(&editor_term_text_change),
		EDITOR_CURSOR_LISTENER(&editor_term_cursor_change),
		term,
		editor,
		size,
		VEC2U(0, 0),
		VECTOR(t_editor_term_update),
	};
}
