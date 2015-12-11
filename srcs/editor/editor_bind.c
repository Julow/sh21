/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bind.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:29 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/11 17:00:34 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

void		editor_bind(t_editor *editor,
				t_key key, bool (*f)(t_editor*, t_key))
{
	t_binding *const	binding = ft_bst_put(&editor->bindings, &key, 0);

	binding->key = key;
	binding->f = f;
}
