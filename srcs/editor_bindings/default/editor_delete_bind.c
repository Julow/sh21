/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_delete_bind.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 17:56:49 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 19:42:11 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_default_bindings.h"

static t_editor_sel	delete_text(t_editor *e, t_editor_sel sel,
						t_editor_sel *batch, uint32_t *cursor_offset)
{
	if (batch->x != batch->y)
		editor_write(e, *batch, SUB0(), true);
	sel = EDITOR_SEL_NORM(sel);
	sel.x -= *cursor_offset;
	sel.y -= *cursor_offset;
	if (sel.x != sel.y)
		*batch = sel;
	return (EDITOR_SEL(sel.x, sel.x));
}

static void		delete_sel(t_editor *e, t_editor_sel *cursors,
					t_editor_sel *batch)
{
	uint32_t		i;
	uint32_t		cursor_offset;

	cursor_offset = 0;
	i = 0;
	while (i < e->cursors.length)
	{
		cursors[i] = delete_text(e, EDITOR_CURSOR(e, i), batch, &cursor_offset);
		i++;
	}
}

static void		delete_move(t_editor_delete_bind *b, t_editor *e,
					t_editor_sel *cursors, t_editor_sel *batch)
{
	uint32_t		i;
	uint32_t		cursor_offset;

	cursor_offset = 0;
	i = 0;
	while (i < e->cursors.length)
	{
		cursors[i].x = EDITOR_CURSOR(e, i).x;
		cursors[i].y = b->move_f(e, cursors[i].x,
						b->flags & EDITOR_DELETE_BACKWARD);
		cursors[i] = delete_text(e, cursors[i], batch, &cursor_offset);
		i++;
	}
}

bool			editor_delete_bind(t_editor_delete_bind *b, t_editor *e)
{
	t_editor_sel	cursors[e->cursors.length];
	t_editor_sel	batch;

	batch = EDITOR_SEL(0, 0);
	if (b->flags & EDITOR_DELETE_SEL)
		delete_sel(e, cursors, &batch);
	if (batch.x == batch.y)
		delete_move(b, e, cursors, &batch);
	if (batch.x == batch.y)
		return (false);
	editor_write(e, batch, SUB0(), false);
	editor_set_cursors(e, cursors, ARRAY_LEN(cursors), false);
	return (true);
}
