/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/11 18:44:55 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/18 11:05:19 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/file_in.h"
#include "ft/ft_printf.h"
#include "sh/ast.h"
#include "sh/exec.h"
#include "sh/parser.h"

#include "p_default_builtins.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int		sh_exec_file(t_sh_context *c, t_in *in)
{
	t_sh_compound	compound;
	int				status;
	t_sh_parse_err	*err;

	status = 0;
	while (IN_REFRESH(in))
	{
		if ((err = sh_parse(&c->parser_context, in, &compound)) != NULL)
		{
			sh_print_parse_err(err);
			free(err);
			status = 1;
			break ;
		}
		status = sh_exec_compound(c, &compound, false);
		sh_destroy_compound(&compound);
	}
	return (status);
}

int				sh_builtin_source(t_sh_context *c, void *data, t_argv args)
{
	t_file_in		*file;
	int				status;

	if (args.argc <= 1)
	{
		ft_dprintf(2, "%s: not enough argument%n", args.argv[0]);
		ft_dprintf(2, "usage: %s file%n", args.argv[0]);
		return (2);
	}
	if ((file = ft_in_open(ft_sub(args.argv[1], 0, -1))) == NULL)
	{
		ft_dprintf(2, "%s: %s: %s%n",
			args.argv[0], args.argv[1], strerror(errno));
		return (1);
	}
	status = sh_exec_file(c, &file->in);
	ft_in_close(file);
	return (status);
	(void)data;
}
