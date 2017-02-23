/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 15:21:11 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:17:24 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_vec2u			editor_getpos(t_editor const *editor, uint32_t pos)
{
	uint32_t				i;
	uint32_t const *const	line_lengths = editor->lines.data;

	i = 0;
	while (line_lengths[i] < pos)
		pos -= line_lengths[i++];
	if (line_lengths[i] == pos && i < editor->lines.length - 1)
		return (VEC2U(i + 1, 0));
	return (VEC2U(i, pos));
}

uint32_t		editor_getindex(t_editor const *editor, t_vec2u pos)
{
	uint32_t		index;

	index = 0;
	while (pos.x-- > 0)
		index += EDITOR_LINE(editor, pos.x);
	return (index + pos.y);
}

void			editor_register_listener(t_editor *editor,
					t_editor_text_listener *text_l,
					t_editor_cursor_listener *cursor_l)
{
	if (text_l != NULL)
		ft_vpush(&editor->text_listeners, &text_l, 1);
	if (cursor_l != NULL)
		ft_vpush(&editor->cursor_listeners, &cursor_l, 1);
}
