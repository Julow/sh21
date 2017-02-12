/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 19:25:13 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/12 23:26:52 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void		notify_text_listeners(t_vector *listeners,
					t_editor_sel sel, t_sub new_text, bool batch)
{
	t_editor_text_listener	*l;

	l = VECTOR_IT(*listeners);
	while (VECTOR_NEXT(*listeners, l))
		l->on_change(l, sel, new_text, batch);
}

/*
** The size of the lines in 'text' are added (+=) to the numbers in 'add'
** If 'add' is NULL, do nothing
** Return the number of line in 'text'
*/
static uint32_t	line_lengths(t_sub text, uint32_t *add)
{
	uint32_t		i;
	uint32_t		offset;
	uint32_t		tmp;

	i = 0;
	offset = 0;
	while (true)
	{
		if ((tmp = ft_subfind_c(text, '\n', offset)) >= text.length)
			break ;
		tmp++;
		if (add != NULL)
			add[i] += tmp - offset;
		i++;
		offset = tmp;
	}
	if (add != NULL)
		add[i] += tmp - offset;
	return (i + 1);
}

void			editor_write(t_editor *editor, t_editor_sel sel, t_sub text,
					bool batch)
{
	t_vec2u			a;
	t_vec2u			b;
	uint32_t		count;

	sel = EDITOR_SEL_NORM(sel);
	notify_text_listeners(&editor->text_listeners, sel, text, batch);
	ft_memcpy(ft_dstrspan(&editor->text, sel.x, sel.y, text.length),
			text.str, text.length);
	a = editor_getpos(editor, sel.x);
	b = editor_getpos(editor, sel.y);
	count = line_lengths(text, NULL);
	ft_vspan(&editor->lines, VEC2U(a.x, b.x), NULL, count - 1);
	memset(&VGET(uint32_t, editor->lines, a.x), 0, S(uint32_t, count - 1));
	VGET(uint32_t, editor->lines, a.x) += a.y;
	VGET(uint32_t, editor->lines, a.x + count - 1) -= b.y;
	line_lengths(text, &VGET(uint32_t, editor->lines, a.x));
}
