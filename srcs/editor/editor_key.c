/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 14:40:33 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

bool		editor_key(t_editor *editor, t_key key)
{
	t_binding	*binding;
	char		c;

	key.mods |= editor->extra_mods;
	editor->extra_mods = 0;
	binding = ft_bst_get(&editor->bindings, NULL, &key);
	if (binding != NULL)
		return (binding->f(editor, binding->flags));
	if (!IS(key.c, IS_PRINT) || key.mods & ~KEY_MOD_SHIFT)
		return (false);
	c = key.c;
	editor_write(editor, VEC2U(editor->cursor, editor->cursor + editor->sel),
		SUB(&c, 1));
	editor_set_cursor(editor, editor->cursor + 1, 0);
	return (true);
}
