/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_set_cursor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:08:37 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 14:17:37 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void		editor_set_cursor(t_editor *editor, uint32_t cursor, int32_t sel)
{
	t_vec2u			range;

	ft_spanlist_clear(&editor->spans, EDITOR_SEL_PRIORITY);
	editor->cursor = cursor;
	editor->sel = sel;
	if (sel == 0)
		return ;
	if (sel < 0)
		range = VEC2U(editor->cursor + sel, editor->cursor);
	else
		range = VEC2U(editor->cursor, editor->cursor + sel);
	ft_spanlist_set(&editor->spans, range, EDITOR_SEL_PRIORITY);
}
