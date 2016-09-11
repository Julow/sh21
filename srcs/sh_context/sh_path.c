/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/11 19:20:48 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/11 19:31:48 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_context.h"
#include <unistd.h>

bool			sh_c_path_search(t_sh_context *c, t_sub path,
					t_sub name, t_dstr *dst)
{
	t_sub			sub;

	if (ft_subfind_c(name, '/', 0) < name.length)
	{
		ft_dstradd(dst, name);
		return (true);
	}
	sub = SUB_START(path);
	while (ft_subnext_c(path, &sub, ':'))
	{
		if (sub.length == 0)
			continue ;
		dst->length = 0;
		ft_dstradd(dst, sub);
		if (dst->str[dst->length - 1] != '/')
			ft_dstradd(dst, SUBC("/"));
		ft_dstradd(dst, name);
		if (access(dst->str, X_OK) == 0)
			return (true);
	}
	return (false);
	(void)c;
}
