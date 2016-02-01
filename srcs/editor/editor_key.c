/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/01 23:43:13 by juloo            ###   ########.fr       */
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
	editor_put(editor, SUB(&c, 1));
	return (true);
}
