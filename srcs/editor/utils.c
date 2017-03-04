/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 15:21:11 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/04 17:05:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void			editor_init(t_editor *dst)
{
	*dst = (t_editor){
		DSTR0(),
		SUMSET(),
		VECTOR(t_editor_sel),
		VECTOR(t_editor_text_listener*),
		VECTOR(t_editor_cursor_listener*),
	};
	ft_sumset_insert(&dst->lines, 0, 0);
	*(t_editor_sel*)ft_vpush(&dst->cursors, NULL, 1) = EDITOR_SEL(0, 0);
}

t_vec2u			editor_getpos(t_editor const *editor, uint32_t pos)
{
	t_vec2u const		tmp = ft_sumset_index(&editor->lines, pos);

	if (tmp.x >= SUMSET_LENGTH(editor->lines))
		return (VEC2U(tmp.x - 1, ft_sumset_get(&editor->lines, tmp.x - 1).y));
	return (tmp);
}

uint32_t		editor_getindex(t_editor const *editor, t_vec2u pos)
{
	return (ft_sumset_get(&editor->lines, pos.x).x + pos.y);
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
