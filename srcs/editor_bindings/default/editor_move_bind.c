/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_move_bind.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:36:13 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 18:44:11 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_default_bindings.h"

bool			editor_move_bind(t_editor_move_bind *b, t_editor *e)
{
	t_editor_sel	cursors[e->cursors.length * 2];
	uint32_t		i;
	bool			moved;
	uint32_t		tmp;

	moved = false;
	i = 0;
	while (i < e->cursors.length)
	{
		cursors[i] = VGETC(t_editor_sel, e->cursors, i);
		tmp = b->move_f(e, cursors[i].x, b->flags & EDITOR_MOVE_BACKWARD);
		moved |= (tmp != cursors[i].x);
		cursors[i + e->cursors.length] = (b->flags & EDITOR_MOVE_SEL)
				? EDITOR_SEL(tmp, cursors[i].y) : EDITOR_SEL(tmp, tmp);
		i++;
	}
	tmp = (b->flags & EDITOR_MOVE_ADD) ? 0 : e->cursors.length;
	if (moved)
		editor_set_cursors(e, cursors + tmp, ARRAY_LEN(cursors) - tmp, false);
	return (moved);
}
