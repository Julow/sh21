/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_var.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 13:45:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 15:23:07 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/context.h"
#include "p_sh_context.h"

#include <stdlib.h>

void			sh_var_set(t_sh_context *c, t_sub key, t_sub value)
{
	t_sh_var		*var;
	char			*dst;
	uint32_t const	len = key.length + value.length + 1;

	if ((var = ft_set_get(&c->vars, &key)) == NULL || len > var->length)
	{
		if (var != NULL)
		{
			ft_set_remove(&c->vars, var);
			free(var);
		}
		var = MALLOC(sizeof(t_sh_var) + len + 1);
		ft_set_insert(&c->vars, var, &key);
	}
	*var = SH_VAR(len, key.length);
	dst = SH_VAR_STR(var);
	ft_memcpy(dst, key.str, key.length);
	dst[key.length] = '=';
	ft_memcpy(dst + key.length + 1, value.str, value.length);
	dst[len] = '\0';
}

void			sh_var_unset(t_sh_context *c, t_sub key)
{
	t_sh_var *const	var = ft_set_get(&c->vars, &key);

	if (var != NULL)
	{
		ft_set_remove(&c->vars, var);
		free(var);
	}
}

t_sub			sh_var_get(t_sh_context *c, t_sub key)
{
	t_sh_var *const	var = ft_set_get(&c->vars, &key);

	if (var == NULL)
		return (SUB0());
	return (SUB_FOR(SUB(SH_VAR_STR(var), var->length), var->key_len + 1));
}

int				sh_var_cmp(t_sh_var const *v, t_sub const *key)
{
	if (v->key_len != key->length)
		return (v->key_len != key->length);
	return (ft_memcmp(SH_VAR_STR(v), key->str, key->length));
}
