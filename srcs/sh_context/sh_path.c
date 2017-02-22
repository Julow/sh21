/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/11 19:20:48 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:32:47 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "p_sh_context.h"

#include <stdlib.h>
#include <unistd.h>

bool			sh_c_path_search(t_sh_context *c, t_sub name, t_dstr *dst)
{
	uint32_t const	begin = dst->length;
	t_sub			path;
	t_sub			sub;

	if (name.length == 0 || ft_subfind_c(name, '/', 0) < name.length)
	{
		ft_dstradd(dst, name);
		return (true);
	}
	path = sh_c_path_get(c, name);
	if (path.length != 0)
	{
		ft_dstradd(dst, path);
		return (true);
	}
	path = sh_c_var_get(c, SUBC("PATH"));
	sub = SUB_START(path);
	while (ft_subnext_c(path, &sub, ':'))
	{
		dst->length = begin;
		ft_dstradd(dst, (sub.length == 0) ? SUBC("./") : sub);
		if (dst->str[dst->length - 1] != '/')
			ft_dstradd(dst, SUBC("/"));
		ft_dstradd(dst, name);
		if (access(dst->str + begin, X_OK) == 0)
		{
			sh_c_path_set(c, name, SUB_FOR(DSTR_SUB(*dst), begin));
			return (true);
		}
	}
	return (false);
}

void			sh_c_path_clear(t_sh_context *c)
{
	t_sh_c_path		*p;

	while ((p = c->path_cache.root) != NULL)
	{
		ft_set_remove(&c->path_cache, p);
		free(p);
	}
}

void			sh_c_path_set(t_sh_context *c, t_sub name, t_sub path)
{
	t_sh_c_path		*p;

	sh_c_path_remove(c, name);
	p = MALLOC(sizeof(t_sh_c_path) + name.length + path.length);
	p->set_h = SET_HEAD();
	p->name = SUB_DST(ENDOF(p), name);
	p->path = SUB_DST(ENDOF(p) + name.length, path);
	ft_set_insert(&c->path_cache, p, &p->name);
}

t_sub			sh_c_path_get(t_sh_context const *c, t_sub name)
{
	t_sh_c_path const *const	p = ft_set_cget(&c->path_cache, &name);

	return ((p == NULL) ? SUB0() : p->path);
}

bool			sh_c_path_remove(t_sh_context *c, t_sub name)
{
	t_sh_c_path *const		p = ft_set_get(&c->path_cache, &name);

	if (p == NULL)
		return (false);
	ft_set_remove(&c->path_cache, p);
	free(p);
	return (true);
}

int				sh_c_path_cmp(t_sh_c_path const *v, t_sub const *key)
{
	return (SUB_CMP(v->name, *key));
}
