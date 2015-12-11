/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:08 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/11 17:17:47 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

bool		editor_key(t_editor *editor, t_key key)
{
	t_binding *const	binding = ft_bst_get(&editor->bindings, &key);

	if (binding != NULL)
		return (binding->f(editor, key));
	if (!IS(key.c, IS_PRINT) || key.mods & (KEY_MOD_ALT | KEY_MOD_CTRL))
		return (false);
	*ft_dstrspan(&editor->text, editor->cursor,
		editor->cursor + editor->sel, 1) = key.c;
	editor->cursor++;
	editor->sel = 0;
	return (true);
}
