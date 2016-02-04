/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 13:06:01 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:21:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool		editor_bind_paste(void *flags, t_editor *editor)
{
	t_sub *const	paste = editor->clipboard.first;

	if (paste == NULL)
		return (false);
	editor_write(editor, VEC2U(editor->cursor, editor->cursor + editor->sel),
		*paste);
	editor_set_cursor(editor, editor->cursor + paste->length, 0);
	if (((uintptr_t)flags) & PASTE_CONSUME)
		ft_listremove(&editor->clipboard, editor->clipboard.first);
	return (true);
}
