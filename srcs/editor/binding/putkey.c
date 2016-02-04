/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putkey.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 12:41:19 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:47:19 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

bool		editor_bind_putkey(void *data, t_editor *editor, t_key key)
{
	char		c;

	if (!IS(key.c, IS_PRINT))
		return (false);
	c = key.c;
	editor_write(editor,
		VEC2U(editor->cursor, editor->cursor + editor->sel), SUB(&c, 1));
	editor_set_cursor(editor, editor->cursor + 1, 0);
	return (true);
	(void)data;
}
