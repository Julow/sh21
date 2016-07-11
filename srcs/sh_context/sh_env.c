/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 14:06:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 15:13:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/context.h"
#include "p_sh_context.h"

void			sh_env_build(t_sh_context const *c, char const **env)
{
	t_strset_node const	*key;
	t_sh_var const		*var;

	key = ft_set_cfirst(&c->env_keys, NULL);
	while (key != NULL)
	{
		var = ft_set_cget(&c->vars, &key->value);
		HARD_ASSERT(var != NULL);
		*env = SH_VAR_STR(var);
		env++;
		key = ft_set_cnext(key);
	}
}

void			sh_env_export(t_sh_context *c, t_sub key, bool exported)
{
	t_strset_node	*tmp;

	if (((tmp = ft_set_get(&c->env_keys, &key)) != NULL) == exported)
		return ;
	if (exported)
		ft_set_insert(&c->env_keys, ft_strset_node(key), &key);
	else
		ft_set_remove(&c->env_keys, tmp);
}
