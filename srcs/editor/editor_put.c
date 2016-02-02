/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/01 22:09:25 by juloo             #+#    #+#             */
/*   Updated: 2016/02/03 00:24:41 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

#define VEC2U1(XY)		VEC2U(XY, XY)

#define EDITOR_LINE(E,Y)	((uint32_t*)VECTOR_GET((E).line_stops, (Y)))

void			editor_erase(t_editor *editor, t_vec2u range)
{
	t_vec2u const	begin = editor_rowcol(editor, range.x);
	t_vec2u const	end = editor_rowcol(editor, range.y);

	ft_dstrspan(&editor->text, range.x, range.y, 0);
	if (begin.y == end.y)
		*EDITOR_LINE(*editor, begin.y) -= end.x - begin.x;
	else
	{
		*EDITOR_LINE(*editor, begin.y) = begin.x;
		*EDITOR_LINE(*editor, end.y) -= end.x;
		ft_vspan(&editor->line_stops, VEC2U(begin.y + 1, end.y - 1), NULL, 0);
	}
}

void			editor_put(t_editor *editor, t_sub str)
{
	uint32_t		line_count;
	uint32_t		*stops;
	uint32_t		i;
	uint32_t		tmp;

	if (editor->sel != 0)
		editor_erase(editor, VEC2U(editor->cursor + MIN(editor->sel, 0),
			editor->cursor + MAX(editor->sel, 0)));
	i = 0;
	line_count = 0;
	while (i < str.length)
		if (str.str[i++] == '\n')
			line_count++;
	tmp = editor_rowcol(editor, editor->cursor).y;
	if (line_count > 0)
		stops = ft_vspan(&editor->line_stops, VEC2U(tmp, tmp), NULL, line_count);
	else
		stops = EDITOR_LINE(*editor, tmp + 1);
	stops--;
	i = 0;
	tmp = 0;
	while (i < str.length)
	{
		if (str.str[i] == '\n')
		{
			if (tmp == 0)
				*stops -= str.length - i;
			else
				*stops = i - tmp;
			stops++;
			tmp = i;
		}
		i++;
	}
	*stops += i - tmp;
	ft_memcpy(ft_dstrspan(&editor->text, editor->cursor, editor->cursor,
		str.length), str.str, str.length);
	editor_set_cursor(editor, editor->cursor + str.length, 0);
}
