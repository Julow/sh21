/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:25:33 by juloo             #+#    #+#             */
/*   Updated: 2016/02/04 12:20:30 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool			editor_bind_cursor_move(void *data, t_editor *editor)
{
	uintptr_t const	flags = (uintptr_t)data;
	int32_t const	move = cursor_move(editor, flags);

	if (editor->sel != 0 && !(flags & CURSOR_MOVE_SEL))
	{
		if (flags & CURSOR_MOVE_BOUND)
			editor_set_cursor(editor, editor->cursor + move, 0);
		else if (INT_NORM(editor->sel) == INT_NORM(move))
			editor_set_cursor(editor, editor->cursor + editor->sel, 0);
		else
			editor_set_cursor(editor, editor->cursor, 0);
		return (true);
	}
	if (move == 0 || (((int)editor->cursor) + move) < 0
		|| (editor->cursor + move) > editor->text.length)
		return (false);
	editor_set_cursor(editor, editor->cursor + move,
		((flags & CURSOR_MOVE_SEL) ? editor->sel - move : editor->sel));
	return (true);
}
