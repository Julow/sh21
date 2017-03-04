/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 19:25:13 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/04 17:15:39 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void		notify_text_listeners(t_vector *listeners,
					t_editor_text_event const *event, bool batch)
{
	t_editor_text_listener	**l;

	l = VECTOR_IT(*listeners);
	while (VECTOR_NEXT(*listeners, l))
		(*l)->on_change(*l, event, batch);
}

static uint32_t	count_lines(t_sub text)
{
	uint32_t		offset;
	uint32_t		count;

	count = 1;
	offset = 0;
	while ((offset = ft_subfind_c(text, '\n', offset)) < text.length)
	{
		count++;
		offset++;
	}
	return (count);
}

static void		span_lines(t_editor *editor, t_vec2u range, uint32_t size)
{
	uint32_t const	end = range.x + size;
	uint32_t		i;

	while (range.y > end)
		ft_sumset_remove(&editor->lines, --range.y);
	i = range.x;
	while (i < range.y)
		ft_sumset_set(&editor->lines, i++, 0);
	while (range.y < end)
		ft_sumset_insert(&editor->lines, range.y++, 0);
}

static uint32_t	add_line_lengths(t_editor *editor, t_sub text, uint32_t i)
{
	uint32_t		offset;
	uint32_t		tmp;

	offset = 0;
	while ((tmp = ft_subfind_c(text, '\n', offset)) < text.length)
	{
		ft_sumset_add(&editor->lines, i, tmp + 1 - offset);
		offset = tmp + 1;
		i++;
	}
	return (tmp - offset);
}

void			editor_write(t_editor *editor, t_vec2u range, t_sub text,
					bool batch)
{
	t_vec2u			a;
	t_vec2u			b;
	uint32_t const	line_count = count_lines(text);
	uint32_t		tmp;

	range = EDITOR_SEL_NORM(range);
	a = editor_getpos(editor, range.x);
	b = (range.x == range.y) ? a : editor_getpos(editor, range.y);
	notify_text_listeners(&editor->text_listeners,
			&(t_editor_text_event){ range, a, b, text, line_count }, batch);
	ft_dstrspan(&editor->text, range.x, range.y, text.str, text.length);
	span_lines(editor, VEC2U(a.x, b.x), line_count - 1);
	tmp = add_line_lengths(editor, text, a.x);
	if (line_count <= 1)
	{
		ft_sumset_add(&editor->lines, a.x, a.y - b.y + tmp);
	}
	else
	{
		ft_sumset_add(&editor->lines, a.x, a.y);
		ft_sumset_add(&editor->lines, a.x + line_count - 1, tmp - b.y);
	}
}
