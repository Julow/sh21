/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_function.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/10 11:09:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/10 23:23:17 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "p_sh_context.h"

#include <stdlib.h>

t_sh_cmd const	*sh_c_function_get(t_sh_context const *c, t_sub name)
{
	t_sh_c_function const *const	f = ft_set_cget(&c->functions, &name);

	return ((f == NULL) ? NULL : &f->body);
}

void			sh_c_function_define(t_sh_context *c, t_sh_func_def const *f)
{
	t_sh_c_function		*c_func;

	if ((c_func = ft_set_get(&c->functions, &f->name)) != NULL)
	{
		ft_set_remove(&c->functions, c_func);
		sh_destroy_cmd(&c_func->body);
		free(c_func);
	}
	c_func = MALLOC(sizeof(t_sh_c_function) + f->name.length);
	c_func->set_h = SET_HEAD();
	c_func->name = SUB_DST(ENDOF(c_func), f->name);
	sh_copy_cmd(&f->body, &c_func->body);
	ft_set_insert(&c->functions, c_func, &c_func->name);
}

int				sh_c_function_cmp(t_sh_c_function const *v, t_sub const *key)
{
	return (SUB_CMP(v->name, *key));
}
