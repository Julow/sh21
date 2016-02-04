/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binding_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 18:09:16 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 16:27:38 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

int32_t			cursor_move(t_editor *editor, uint32_t flags)
{
	t_vec2u			tmp;

	if (flags & CURSOR_MOVE_BOUND)
		return ((flags & CURSOR_MOVE_LEFT) ? -editor->cursor
				: editor->text.length - editor->cursor);
	if (flags & CURSOR_MOVE_LINEBOUND)
	{
		tmp = editor_rowcol(editor, editor->cursor);
		return ((flags & CURSOR_MOVE_LEFT) ? -tmp.x
			: EDITOR_LINE(editor, tmp.y) - tmp.x - 1);
	}
	if (flags & CURSOR_MOVE_LINE)
	{
		tmp = editor_rowcol(editor, editor->cursor);
		tmp.y += (flags & CURSOR_MOVE_LEFT) ? -1 : 1;
		if (tmp.y >= editor->line_stops.length)
			return (0);
		if (EDITOR_LINE(editor, tmp.y) <= tmp.x)
			tmp.x = EDITOR_LINE(editor, tmp.y) - 1;
		return (editor_index(editor, tmp) - editor->cursor);
	}
	if (flags & CURSOR_MOVE_WORD)
	{
		if (flags & CURSOR_MOVE_LEFT && editor->cursor > 0)
			tmp.x = editor->cursor - 1;
		else
			tmp.x = editor->cursor;
		tmp = ft_word_range(*(t_sub*)&editor->text, tmp.x,
			BOOL_OF(flags & _CURSOR_MOVE_SUBWORD));
		return (((flags & CURSOR_MOVE_LEFT) ? tmp.x : tmp.y)
			- editor->cursor);
	}
	return ((flags & CURSOR_MOVE_LEFT) ? -1 : 1);
}
