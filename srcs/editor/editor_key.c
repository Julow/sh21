/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:40:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

static t_binding const	*get_binding(t_editor const *editor, t_key key)
{
	t_binding const *const	*b;

	if ((b = ft_bst_get(&editor->bindings, NULL, &key)) == NULL)
		return (NULL);
	return (*b);
}

bool					editor_key(t_editor *editor, t_key key)
{
	t_binding const			*binding;

	key.mods |= editor->extra_mods;
	editor->extra_mods = 0;
	binding = get_binding(editor, key);
	if (binding == NULL)
		binding = get_binding(editor, KEY(-1, key.mods));
	while (binding != NULL)
		if (CALL(bool, binding->callback, editor, key))
			return (true);
		else
			binding = binding->next;
	return (false);
}
