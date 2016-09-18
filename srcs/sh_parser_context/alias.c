/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/18 10:51:26 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/18 11:05:49 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "sh/parser_context.h"

#include <stdlib.h>

void			sh_c_alias_set(t_sh_parser_context *c, t_sub name, t_sub val)
{
	t_sh_c_alias	*alias;

	sh_c_alias_unset(c, name);
	alias = MALLOC(sizeof(t_sh_c_alias) + name.length + val.length);
	*alias = (t_sh_c_alias){
		SET_HEAD(),
		SUB_DST(ENDOF(alias), name),
		SUB_DST(ENDOF(alias) + name.length, val)
	};
	ft_set_insert(&c->aliases, alias, &alias->name);
}

t_sub const		*sh_c_alias_get(t_sh_parser_context const *c, t_sub name)
{
	t_sh_c_alias const *const	alias = ft_set_cget(&c->aliases, &name);

	return ((alias == NULL) ? NULL : &alias->val);
}

bool			sh_c_alias_unset(t_sh_parser_context *c, t_sub name)
{
	t_sh_c_alias *const	alias = ft_set_get(&c->aliases, &name);

	if (alias == NULL)
		return (false);
	ft_set_remove(&c->aliases, alias);
	free(alias);
	return (true);
}

int				sh_c_alias_cmp(t_sh_c_alias const *v, t_sub const *key)
{
	return (SUB_CMP(v->name, *key));
}
