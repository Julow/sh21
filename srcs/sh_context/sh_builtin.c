/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 15:28:33 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:36:40 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_context.h"
#include <stdlib.h>

bool			sh_c_builtin_get(t_sh_context const *c, t_sub name,
					t_sh_builtin *builtin, void **builtin_data)
{
	t_sh_c_builtin const	*node;

	if ((node = ft_set_cget(&c->builtins, &name)) == NULL)
		return (false);
	*builtin = node->f;
	*builtin_data = ENDOF(node);
	return (true);
}

void			*sh_c_builtin_register(t_sh_context *c, t_sub name,
					t_sh_builtin builtin_f, uint32_t data_size)
{
	t_sh_c_builtin			*node;

	node = MALLOC(sizeof(t_sh_c_builtin) + data_size + name.length);
	*node = (t_sh_c_builtin){
		SET_HEAD(),
		SUB(ENDOF(node) + data_size, name.length),
		builtin_f
	};
	memcpy(ENDOF(node) + data_size, name.str, name.length);
	ft_set_insert(&c->builtins, node, &name);
	return (ENDOF(node));
}

void			sh_c_builtin_unregister(t_sh_context *c, t_sub name,
					void (*clean)(void *data))
{
	t_sh_c_builtin			*node;

	if ((node = ft_set_get(&c->builtins, &name)) == NULL)
		return ;
	if (clean != NULL)
		clean(ENDOF(node));
	free(node);
	ft_set_remove(&c->builtins, node);
}

int				sh_c_builtin_cmp(t_sh_c_builtin const *v, t_sub const *key)
{
	return (SUB_CMP(v->name, *key));
}
