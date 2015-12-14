/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:25:33 by juloo             #+#    #+#             */
/*   Updated: 2015/12/14 18:46:30 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool			editor_bind_cursor_move(t_editor *editor, uint32_t flags)
{
	int32_t const	move = cursor_move(editor, flags);

	if (editor->sel != 0 && !(flags & CURSOR_MOVE_SEL))
	{
		if (flags & CURSOR_MOVE_BOUND)
			editor->cursor += move;
		else if (INT_NORM(editor->sel) == INT_NORM(move))
			editor->cursor += editor->sel;
		editor->sel = 0;
		return (true);
	}
	if (move == 0 || (editor->cursor + move) < 0
		|| (editor->cursor + move) > editor->text.length)
		return (false);
	editor->cursor += move;
	if (flags & CURSOR_MOVE_SEL)
		editor->sel -= move;
	return (true);
}
