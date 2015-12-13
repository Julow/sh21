/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:25:33 by juloo             #+#    #+#             */
/*   Updated: 2015/12/12 23:47:26 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"

bool			editor_bind_cursor_begin(t_editor *editor, t_key key)
{
	if (editor->cursor == 0)
		return (false);
	editor->cursor = 0;
	return (true);
	(void)key;
}

bool			editor_bind_cursor_end(t_editor *editor, t_key key)
{
	uint32_t const	end = editor->text.length;

	if (editor->cursor == end)
		return (false);
	editor->cursor = end;
	return (true);
	(void)key;
}

bool			editor_bind_cursor_left(t_editor *editor, t_key key)
{
	if (editor->cursor == 0)
		return (false);
	editor->cursor--;
	return (true);
	(void)key;
}

bool			editor_bind_cursor_right(t_editor *editor, t_key key)
{
	uint32_t const	end = editor->text.length;

	if (editor->cursor == end)
		return (false);
	editor->cursor++;
	return (true);
	(void)key;
}
