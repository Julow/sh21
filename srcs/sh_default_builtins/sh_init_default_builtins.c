/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_init_default_builtins.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:13:18 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 16:41:09 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_default_builtins.h"

int				sh_builtin_echo(t_sh_context *c, void *data, // TODO: TMP
					uint32_t argc, char const *const *argv)
{
	ft_printf("ECHO ECHO%n");
	return (0);
	(void)c;
	(void)data;
	(void)argc;
	(void)argv;
}

t_sh_builtin_def const	g_sh_default_builtins[] = {
	{SUBC("echo"), &sh_builtin_echo},
};

void			sh_init_default_builtins(t_sh_context *c)
{
	uint32_t		i;

	i = 0;
	while (i < ARRAY_LEN(g_sh_default_builtins))
	{
		sh_c_builtin_register(c, g_sh_default_builtins[i].name,
			g_sh_default_builtins[i].builtin_f, 0);
		i++;
	}
}
