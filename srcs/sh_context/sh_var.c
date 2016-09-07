/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_var.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 13:45:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:37:36 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/context.h"
#include "p_sh_context.h"

#include <stdlib.h>

void			sh_c_var_set(t_sh_context *c, t_sub key, t_sub value)
{
	t_sh_c_var		*var;
	char			*dst;
	uint32_t const	len = key.length + value.length + 1;

	if ((var = ft_set_get(&c->vars, &key)) == NULL || len > var->length)
	{
		if (var != NULL)
		{
			ft_set_remove(&c->vars, var);
			free(var);
		}
		var = MALLOC(sizeof(t_sh_c_var) + len + 1);
		*var = SH_C_VAR(0, 0);
		ft_set_insert(&c->vars, var, &key);
	}
	var->length = len;
	var->key_len = key.length;
	dst = SH_C_VAR_STR(var);
	ft_memcpy(dst, key.str, key.length);
	dst[key.length] = '=';
	ft_memcpy(dst + key.length + 1, value.str, value.length);
	dst[len] = '\0';
}

void			sh_c_var_unset(t_sh_context *c, t_sub key)
{
	t_sh_c_var *const		var = ft_set_get(&c->vars, &key);
	t_strset_node *const	env_key = ft_set_get(&c->env_keys, &key);

	if (var != NULL)
	{
		ft_set_remove(&c->vars, var);
		free(var);
	}
	if (env_key != NULL)
	{
		ft_set_remove(&c->env_keys, env_key);
		free(env_key);
	}
}

t_sub			sh_c_var_get(t_sh_context const *c, t_sub key)
{
	t_sh_c_var const *const	var = ft_set_cget(&c->vars, &key);

	if (var == NULL)
		return (SUB0());
	return (SUB_FOR(SUB(SH_C_VAR_STR(var), var->length), var->key_len + 1));
}

int				sh_c_var_cmp(t_sh_c_var const *v, t_sub const *key)
{
	if (v->key_len != key->length)
		return (v->key_len - key->length);
	return (ft_memcmp(SH_C_VAR_STR(v), key->str, key->length));
}
