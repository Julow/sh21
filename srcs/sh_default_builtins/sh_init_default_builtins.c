/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_init_default_builtins.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:13:18 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:58:32 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_default_builtins.h"

t_sh_builtin_def const	g_sh_default_builtins[] = {
	{SUBC("echo"), &sh_builtin_echo},
	{SUBC("true"), &sh_builtin_true},
	{SUBC(":"), &sh_builtin_true},
	{SUBC("false"), &sh_builtin_false},
	{SUBC("exit"), &sh_builtin_exit},
	{SUBC("pwd"), &sh_builtin_pwd},
	{SUBC("set"), &sh_builtin_set},
	{SUBC("unset"), &sh_builtin_unset},
	{SUBC("shift"), &sh_builtin_shift},
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
