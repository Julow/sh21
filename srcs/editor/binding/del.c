/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 22:00:44 by juloo             #+#    #+#             */
/*   Updated: 2015/12/12 22:09:09 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"

bool			editor_bind_erase_sel(t_editor *editor, t_key key)
{
	if (editor->sel == 0)
		return (false);
	ft_dstrspan(&editor->text, editor->cursor, editor->cursor + editor->sel, 0);
	return (true);
	(void)key;
}

bool			editor_bind_backspace(t_editor *editor, t_key key)
{
	if (editor->sel > 0)
		return (editor_bind_erase_sel(editor, key));
	if (editor->cursor <= 0)
		return (false);
	editor->cursor--;
	ft_dstrspan(&editor->text, editor->cursor, editor->cursor + 1, 0);
	return (true);
}

bool			editor_bind_delete(t_editor *editor, t_key key)
{
	if (editor->sel > 0)
		return (editor_bind_erase_sel(editor, key));
	if (editor->cursor >= (uint32_t)editor->text.length)
		return (false);
	ft_dstrspan(&editor->text, editor->cursor, editor->cursor + 1, 0);
	return (true);
}
