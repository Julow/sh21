/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binding_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 18:09:16 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 19:38:49 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

int32_t			cursor_move(t_editor *editor, uint32_t flags)
{
	t_vec2u			range;

	if (flags & CURSOR_MOVE_BOUND)
		return ((flags & CURSOR_MOVE_LEFT) ? -editor->cursor
				: editor->text.length - editor->cursor);
	if (flags & CURSOR_MOVE_WORD)
	{
		if (flags & CURSOR_MOVE_LEFT && editor->cursor > 0)
			range.x = editor->cursor - 1;
		else
			range.x = editor->cursor;
		range = ft_word_range(*(t_sub*)&editor->text, range.x,
			BOOL_OF(flags & _CURSOR_MOVE_SUBWORD));
		return (((flags & CURSOR_MOVE_LEFT) ? range.x : range.y)
			- editor->cursor);
	}
	return ((flags & CURSOR_MOVE_LEFT) ? -1 : 1);
}
