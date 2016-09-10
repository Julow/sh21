/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/10 23:27:14 by juloo             #+#    #+#             */
/*   Updated: 2016/09/10 23:43:55 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int				sh_builtin_cd(t_sh_context *c, void *data, t_argv args)
{
	char const		*dir;
	char			*pwd;

	if (args.argc > 1)
	{
		if (args.argv[1][0] == '-' && args.argv[1][1] == '\0')
			dir = sh_c_var_get(c, SUBC("OLDPWD")).str;
		else
			dir = args.argv[1];
	}
	else
		dir = sh_c_var_get(c, SUBC("HOME")).str;
	if (chdir(dir) < 0)
	{
		ft_dprintf(2, "cd: %s: %s%n", dir, strerror(errno));
		return (1);
	}
	sh_c_var_set(c, SUBC("OLDPWD"), sh_c_var_get(c, SUBC("PWD")));
	if ((pwd = getcwd(NULL, 0)) != NULL)
	{
		sh_c_var_set(c, SUBC("PWD"), ft_sub(pwd, 0, -1));
		free(pwd);
	}
	return (0);
	(void)data;
}
