/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 13:06:01 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/01 23:42:28 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

bool		editor_bind_paste(t_editor *editor, uint32_t flags)
{
	t_sub *const	paste = editor->clipboard.first;

	if (paste == NULL)
		return (false);
	editor_put(editor, *paste);
	if (flags & PASTE_CONSUME)
		ft_listremove(&editor->clipboard, editor->clipboard.first);
	return (true);
}
