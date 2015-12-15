/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 13:06:01 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/15 13:12:42 by jaguillo         ###   ########.fr       */
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
	if (editor->sel < 0)
		editor->cursor += editor->sel + 1;
	editor->sel = 0;
	editor->cursor += paste->length;
	if (flags & PASTE_CONSUME)
		ft_listremove(&editor->clipboard, editor->clipboard.first);
	return (true);
}
