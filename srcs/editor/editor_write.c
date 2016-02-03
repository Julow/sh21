/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 14:34:17 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 14:38:22 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

#define EDITOR_LINE(E,Y)	((uint32_t*)VECTOR_GET((E).line_stops, (Y)))

static void		editor_erase(t_editor *editor, t_vec2u span)
{
	t_vec2u			begin;
	t_vec2u const	end = editor_rowcol(editor, span.y);

	ft_dstrspan(&editor->text, span.x, span.y, 0);
	begin = editor_rowcol(editor, span.x);
	if (begin.y == end.y)
		*EDITOR_LINE(*editor, begin.y) -= end.x - begin.x;
	else
	{
		*EDITOR_LINE(*editor, begin.y) = begin.x + *EDITOR_LINE(*editor, end.y) - end.x;
		ft_vspan(&editor->line_stops, VEC2U(begin.y + 1, end.y + 1), NULL, 0); // TODO: investigate
	}
}

static uint32_t	*line_stops(t_editor *editor, uint32_t index, t_sub str)
{
	uint32_t		line_count;
	t_vec2u			pos;
	uint32_t		i;
	uint32_t		*stops;

	i = 0;
	line_count = 0;
	while (i < str.length)
		if (str.str[i++] == '\n')
			line_count++;
	pos = editor_rowcol(editor, index - 1); // TODO: ??
	if (line_count == 0)
		return (VECTOR_GET(editor->line_stops, pos.y));
	stops = ft_vspan(&editor->line_stops, VEC2U1(pos.y), NULL, line_count);
	stops[line_count] = stops[0] - pos.x;
	stops[0] = pos.x;
	while (--line_count > 0)
		stops[line_count] = 0;
	return (stops);
}

void			editor_write(t_editor *editor, t_vec2u span, t_sub str)
{
	uint32_t		*stops;
	uint32_t		i;
	uint32_t		tmp;

	if (span.x != span.y)
		editor_erase(editor, (span = N_VEC2U(span.x, span.y)));
	ft_memcpy(ft_dstrspan(&editor->text, span.x, span.x, str.length),
		str.str, str.length);
	stops = line_stops(editor, span.x, str);
	i = 0;
	tmp = 0;
	while (i < str.length)
		if (str.str[i++] == '\n')
		{
			(*stops) += i - tmp;
			stops++;
			tmp = i;
		}
	(*stops) += i - tmp;
}
