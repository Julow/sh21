/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_cursors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 14:17:34 by juloo             #+#    #+#             */
/*   Updated: 2017/02/13 00:03:30 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void		notify_listeners(t_editor *editor, t_editor_sel const *c,
					uint32_t cursor_count, bool batch)
{
	t_editor_cursor_listener	*l;

	l = VECTOR_IT(editor->cursor_listeners);
	while (VECTOR_NEXT(editor->cursor_listeners, l))
		l->on_change(l, c, cursor_count, batch);
}

static int		cursor_cmp(t_editor_sel *data, uint32_t a, uint32_t b)
{
	return ((int)EDITOR_SEL_BEGIN(data[a]) - (int)EDITOR_SEL_BEGIN(data[b]));
}

static void		cursor_swap(t_editor_sel *data, uint32_t a, uint32_t b)
{
	t_editor_sel const	tmp = data[a];

	data[a] = data[b];
	data[b] = tmp;
}

#define SEL_OVERLAP(A, B)	(EDITOR_SEL_END(A) >= EDITOR_SEL_BEGIN(B))

static uint32_t	remove_dup(t_editor_sel *sels, uint32_t count)
{
	uint32_t		i;
	uint32_t		j;
	t_editor_sel	tmp;

	i = 0;
	while (i + 1 < count && !SEL_OVERLAP(sels[i], sels[i + 1]))
		i++;
	j = i;
	while (i + 1 < count)
	{
		tmp = (sels[i].x < sels[i].y) ?
				EDITOR_SEL(sels[i].y, sels[i].x) : sels[i];
		while (i + 1 < count && tmp.x >= EDITOR_SEL_BEGIN(sels[i + 1]))
		{
			tmp.x = EDITOR_SEL_END(sels[i + 1]);
			i++;
		}
		i++;
		sels[j++] = tmp;
		while (i + 1 < count && !SEL_OVERLAP(sels[i], sels[i + 1]))
			sels[j++] = sels[i++];
	}
	if (i < count)
		sels[j++] = sels[i++];
	return (j);
}

void			editor_set_cursors(t_editor *editor, t_editor_sel const *c,
					uint32_t cursor_count, bool batch)
{
	t_editor_sel	sels[cursor_count];

	ft_memcpy(sels, c, sizeof(sels));
	ft_sort(sels, cursor_count, V(&cursor_cmp), V(&cursor_swap));
	cursor_count = remove_dup(sels, cursor_count);
	notify_listeners(editor, sels, cursor_count, batch);
	editor->cursors.length = 0;
	ft_vpush(&editor->cursors, sels, cursor_count);
}
