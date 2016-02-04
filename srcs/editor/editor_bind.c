/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bind.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:29 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:33:47 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

void			editor_bind(t_editor *editor, t_key key,
					t_callback callback, int32_t priority)
{
	t_binding		**b;
	t_binding		*tmp;

	b = ft_bst_get(&editor->bindings, NULL, &key);
	if (b == NULL)
	{
		b = ft_bst_put(&editor->bindings, &key, 0);
		*b = NULL;
	}
	while (*b != NULL && (*b)->priority > priority)
		b = &((*b)->next);
	tmp = NEW(t_binding);
	*tmp = (t_binding){key, callback, priority, *b};
	*b = tmp;
}
