/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 11:31:30 by juloo             #+#    #+#             */
/*   Updated: 2017/02/12 23:32:56 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void			editor_put(t_editor *editor, t_sub text, bool batch)
{
	t_editor_sel	c[editor->cursors.length];
	t_editor_sel	sel;
	int32_t			offset;
	uint32_t		i;

	i = 0;
	offset = 0;
	while (i < editor->cursors.length)
	{
		sel = EDITOR_SEL_NORM(VGET(t_editor_sel, editor->cursors, i));
		sel = EDITOR_SEL(sel.x + offset, sel.y + offset);
		editor_write(editor, sel, text,
				batch || i + 1 < editor->cursors.length);
		c[i] = EDITOR_SEL(sel.x + text.length, sel.x + text.length);
		offset += text.length - (sel.y - sel.x);
		i++;
	}
	editor_set_cursors(editor, c, i, batch);
}
