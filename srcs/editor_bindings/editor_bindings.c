/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 12:31:33 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/17 20:48:56 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"

void			editor_bind(t_editor_binding_set *set, t_key key,
					t_editor_bind *b)
{
	t_editor_bind *const	prev = ft_set_get(&set->bindings, &key);

	if (prev != NULL)
		ft_set_remove(&set->bindings, prev);
	b->key = key;
	b->next = prev;
	ft_set_insert(&set->bindings, b, &key);
}

bool			editor_key(t_editor_binding_set *set, t_editor *e, t_key key)
{
	t_editor_bind	*b;

	b = ft_set_get(&set->bindings, &key);
	while (b != NULL)
	{
		if (b->f(b, e))
			return (true);
		b = b->next;
	}
	return (false);
}

int				editor_bind_cmp(t_editor_bind const *b, t_key const *key)
{
	if (b->key.c != key->c)
		return (b->key.c - key->c);
	return ((int32_t)b->key.mods - (int32_t)key->mods);
}
