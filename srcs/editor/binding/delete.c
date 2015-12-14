/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:00:44 by juloo             #+#    #+#             */
/*   Updated: 2015/12/14 18:32:43 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool			editor_bind_delete(t_editor *editor, uint32_t flags)
{
	int32_t			move;

	move = cursor_move(editor, flags);
	if (editor->sel != 0)
	{
		move = editor->sel;
		editor->sel = 0;
	}
	if (move == 0 || (editor->cursor + move) < 0
		|| (editor->cursor + move) > editor->text.length)
		return (false);
	ft_dstrspan(&editor->text, editor->cursor, editor->cursor + move, 0);
	if (move < 0)
		editor->cursor += move;
	return (true);
}
