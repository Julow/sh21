/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 14:34:17 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 17:58:02 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

#define EDITOR_LINE(E,Y)	((uint32_t*)VECTOR_GET((E).line_stops, (Y)))

static void		editor_erase(t_editor *editor, t_vec2u span)
{
	t_vec2u const	begin = editor_rowcol(editor, span.x);
	t_vec2u const	end = editor_rowcol(editor, span.y);

	ft_dstrspan(&editor->text, span.x, span.y, 0);
	ft_dprintf(2, "ERASE %u,%u -> %u,%u%n", begin.x, begin.y, end.x, end.y);
	if (begin.y >= end.y)
		*EDITOR_LINE(*editor, begin.y) -= end.x - begin.x;
	else
	{
		*EDITOR_LINE(*editor, begin.y) = begin.x + (*EDITOR_LINE(*editor, end.y) - end.x);
		ft_dprintf(2, "REMOVE LINE %u->%u%n", begin.y + 1, end.y + 1);
		ft_bzero(ft_vspan(&editor->line_stops, N_VEC2U(begin.y + 1, end.y + 1), NULL, 0),
			S(uint32_t, end.y - begin.y));
	}
}

void			editor_write(t_editor *editor, t_vec2u span, t_sub str)
{
	uint32_t		y;
	uint32_t		i;
	uint32_t		tmp;

	if (span.x != span.y)
		editor_erase(editor, (span = N_VEC2U(span.x, span.y)));
	ft_memcpy(ft_dstrspan(&editor->text, span.x, span.x, str.length),
		str.str, str.length);
	y = editor_rowcol(editor, span.x).y;
	i = 0;
	tmp = 0;
	while (i < str.length)
	{
		if (str.str[i] == '\n')
		{
			*EDITOR_LINE(*editor, y) += i - tmp;
			y++;
			*(uint32_t*)ft_vspan(&editor->line_stops, VEC2U1(y), NULL, 1) = 0;
			tmp = i;
		}
		i++;
	}
	if (i > tmp)
		*EDITOR_LINE(*editor, y) += i - tmp;
}
