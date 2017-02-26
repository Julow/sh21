/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 17:49:21 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 18:52:06 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_utils.h"

uint32_t		editor_move_line(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	tmp = editor_getpos(e, pos);

	if (b)
	{
		if (tmp.x == 0)
			return (pos);
		return (pos - tmp.y - EDITOR_LINE(e, tmp.x - 1)
				+ MIN(tmp.y, EDITOR_LINE(e, tmp.x - 1)));
	}
	if (tmp.x + 1 >= e->lines.length)
		return (pos);
	return (pos - tmp.y + EDITOR_LINE(e, tmp.x)
			+ MIN(tmp.y, EDITOR_LINE(e, tmp.x + 1)));
}

uint32_t		editor_move_linebound(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	tmp = editor_getpos(e, pos);
	uint32_t		line_len;

	line_len = EDITOR_LINE(e, tmp.x) - (tmp.x + 1 < e->lines.length);
	return (pos - tmp.y + (b ? 0 : line_len));
}

uint32_t		editor_move_bound(t_editor *e, uint32_t pos, bool b)
{
	return (b ? 0 : e->text.length);
}

uint32_t		editor_move_char(t_editor *e, uint32_t pos, bool b)
{
	if (b)
		return ((pos > 0) ? pos - 1 : pos);
	return ((pos < e->text.length) ? pos + 1 : pos);
}
