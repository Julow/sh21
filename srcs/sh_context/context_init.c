/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 15:06:31 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 15:22:44 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/context.h"
#include "p_sh_context.h"

extern char const *const	*environ;

void			sh_context_init(t_sh_context *dst)
{
	uint32_t		i;
	t_sub			sub;
	uint32_t		key_len;

	*dst = SH_CONTEXT();
	i = 0;
	while (environ[i] != NULL)
	{
		sub = ft_sub(environ[i], 0, -1);
		key_len = ft_subfind_c(sub, '=', 0);
		sh_var_set(dst, SUB_LEN(sub, key_len),
			(key_len == sub.length) ? SUB0() : SUB_FOR(sub, key_len + 1));
		sh_env_export(dst, SUB_LEN(sub, key_len), true);
		i++;
	}
}
