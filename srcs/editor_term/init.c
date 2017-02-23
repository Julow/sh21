/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 19:53:22 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/23 19:57:25 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_term.h"
#include "p_editor_term.h"

#include <stddef.h>

static void		editor_term_text_change(t_editor_term *t,
					t_editor_text_event const *e, bool batch)
{
	editor_term_line(t, e->sel_begin.x + 1,
			e->line_count - (e->sel_end.x - e->sel_begin.x + 1));
	// TODO: INSERT_CHAR/REMOVE_CHAR update
	editor_term_redraw(t, e->sel_begin, ft_subfind_c(e->text, '\n', 0));
	(void)batch;
}

static void		update_cursor(t_editor_term *t, t_editor_sel const *sel)
{
	t_vec2u const	a = editor_getpos(t->editor, EDITOR_SEL_BEGIN(*sel));
	t_vec2u const	b = (sel->x == sel->y) ? a
						: editor_getpos(t->editor, EDITOR_SEL_END(*sel));
	uint32_t		line;

	if (a.x == b.x)
		return (editor_term_redraw(t, a, b.y - a.y + 1));
	editor_term_redraw(t, a, EDITOR_LINE(t->editor, a.x) - a.y);
	line = a.x + 1;
	while (line + 1 < b.x)
	{
		editor_term_redraw(t, VEC2U(line, 0), EDITOR_LINE(t->editor, line));
		line++;
	}
	editor_term_redraw(t, VEC2U(b.x, 0), b.y + 1);
}

static void		editor_term_cursor_change(t_editor_term *t,
					t_editor_sel const *new_cursors, uint32_t len, bool batch)
{
	uint32_t			i;
	t_editor_sel const	*sel;

	t = V(t) - offsetof(t_editor_term, cursor_listener);
	sel = VECTOR_GET(t->editor->cursors, 0);
	i = (sel->x == sel->y);
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
	editor_register_listener(editor, &dst->text_listener, 
			&dst->cursor_listener);
}
