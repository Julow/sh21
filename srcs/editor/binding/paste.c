/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 13:06:01 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 14:37:48 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool		editor_bind_paste(t_editor *editor, uint32_t flags)
{
	t_sub *const	paste = editor->clipboard.first;

	if (paste == NULL)
		return (false);
	ft_memcpy(ft_dstrspan(&editor->text, editor->cursor, editor->cursor
		+ editor->sel, paste->length), paste->str, paste->length);
	editor_set_cursor(editor, editor->cursor + paste->length
		+ ((editor->sel < 0) ? editor->sel + 1 : 0), 0);
	if (flags & PASTE_CONSUME)
		ft_listremove(&editor->clipboard, editor->clipboard.first);
	return (true);
}
