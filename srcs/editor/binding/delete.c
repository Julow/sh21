/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:00:44 by juloo             #+#    #+#             */
/*   Updated: 2016/01/26 14:37:15 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool			editor_bind_delete(t_editor *editor, uint32_t flags)
{
	int32_t			move;
	t_sub			*save;

	if (editor->sel != 0)
		move = editor->sel;
	else if ((move = cursor_move(editor, flags)) == 0
		|| (editor->cursor + move) < 0
		|| (editor->cursor + move) > editor->text.length)
		return (false);
	if (flags & DELETE_CLIPBOARD)
	{
		save = ft_listadd(&editor->clipboard, NULL, ABS(move));
		*save = SUB(ENDOF(save), ABS(move));
		ft_memcpy(ENDOF(save),
			editor->text.str + editor->cursor + MIN(move, 0), ABS(move));
	}
	ft_dstrspan(&editor->text, editor->cursor, editor->cursor + move, 0);
	editor_set_cursor(editor,
		((move < 0) ? editor->cursor + move : editor->cursor), 0);
	return (true);
}
