/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_move_bind.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:36:13 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/18 14:48:52 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_default_bindings.h"

uint32_t		editor_move_bind_line(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	tmp = editor_getpos(e, pos);

	if (b)
	{
		if (tmp.x == 0)
			return (pos);
		return (pos - tmp.y - EDITOR_LINE(e, tmp.x - 1)
				+ MIN(tmp.y, EDITOR_LINE(e, tmp.x - 1)));
	}
	if (tmp.x + 1 >= e->lines.length)
		return (pos);
	return (pos - tmp.y + EDITOR_LINE(e, tmp.x)
			+ MIN(tmp.y, EDITOR_LINE(e, tmp.x + 1)));
}

uint32_t		editor_move_bind_linebound(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	tmp = editor_getpos(e, pos);

	return (pos - tmp.y + (b ? MAX(EDITOR_LINE(e, tmp.x), 1) - 1 : 0));
}

uint32_t		editor_move_bind_bound(t_editor *e, uint32_t pos, bool b)
{
	return (b ? 0 : e->text.length);
}

uint32_t		editor_move_bind_char(t_editor *e, uint32_t pos, bool b)
{
	if (b)
		return ((pos > 0) ? pos - 1 : pos);
	return ((pos < e->text.length) ? pos + 1 : pos);
}

bool			editor_move_bind(t_editor_move_bind *b, t_editor *e)
{
	t_editor_sel	cursors[e->cursors.length];
	uint32_t		i;
	bool			moved;
	uint32_t		tmp;
	uint32_t		begin;

	moved = false;
	i = 0;
	while (i < ARRAY_LEN(cursors))
	{
		cursors[i] = VGETC(t_editor_sel, e->cursors, i);
		begin = (b->flags & (EDITOR_MOVE_SEL | EDITOR_MOVE_BACKWARD))
				? cursors[i].y : cursors[i].x;
		tmp = b->move_f(e, begin, b->flags & EDITOR_MOVE_BACKWARD);
		moved |= (tmp != begin);
		cursors[i] = (b->flags & EDITOR_MOVE_SEL)
				? EDITOR_SEL(tmp, cursors[i].y) : EDITOR_SEL(tmp, tmp);
		i++;
	}
	if (moved)
		editor_set_cursors(e, cursors, ARRAY_LEN(cursors), false);
	return (moved);
}
