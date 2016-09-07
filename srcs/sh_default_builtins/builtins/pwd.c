/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 17:58:09 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:25:47 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

int				sh_builtin_pwd(t_sh_context *c, void *data, t_argv args)
{
	char *const		cwd = getcwd(NULL, 0);

	if (cwd == NULL)
	{
		ft_dprintf(2, "pwd: %s%n", strerror(errno));
		return (1);
	}
	ft_printf("%s%n", cwd);
	return (0);
	(void)c;
	(void)data;
	(void)args;
}
