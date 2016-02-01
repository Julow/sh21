/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/01 22:09:25 by juloo             #+#    #+#             */
/*   Updated: 2016/02/01 23:49:35 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

#define VEC2U1(XY)		VEC2U(XY, XY)

void			editor_put(t_editor *editor, t_sub str)
{
	t_vec2u const	sel = (editor->sel < 0)
			? VEC2U(editor->cursor + editor->sel, editor->cursor)
			: VEC2U(editor->cursor, editor->cursor + editor->sel);
	t_vec2u			slines;
	uint32_t		tmp;
	uint32_t		i;

	slines.x = editor_rowcol(editor, sel.x).y;
	slines.y = (editor->sel == 0) ? slines.x : editor_rowcol(editor, sel.y).y;
	i = 0;
	tmp = 0;
	while (str.length > 0)
	{
		if (str.str[i] == '\n' || i >= str.length)
		{
			tmp = i - tmp;
			if (slines.x >= slines.y)
				ft_vspan(&editor->line_stops, VEC2U1(slines.x), &tmp, 1);
			else
				*(uint32_t*)VECTOR_GET(editor->line_stops, slines.x) += tmp;
			tmp = i;
			slines.x++;
			if (i >= str.length)
				break ;
		}
		i++;
	}
	if (slines.x < slines.y)
		ft_vspan(&editor->line_stops, slines, NULL, 0);
	ft_memcpy(ft_dstrspan(&editor->text, sel.x, sel.y, str.length),
		str.str, str.length);
	editor_set_cursor(editor, sel.y + 1, 0);
}
