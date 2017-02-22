/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unalias.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/18 12:56:59 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:35:22 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

static void		remove_all_aliases(t_sh_parser_context *c)
{
	t_sh_c_alias	*alias;

	while ((alias = c->aliases.root) != NULL)
		sh_c_alias_unset(c, alias->name);
}

int				sh_builtin_unalias(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;
	bool			remove_all;
	bool			err;

	remove_all = false;
	while (ft_argv_opt(&args, &arg))
		if (SUB_EQU(arg, SUBC("a")))
			remove_all = true;
		else
		{
			ft_dprintf(2, "unalias: %ts: Unknown option%n", arg);
			return (2);
		}
	if (remove_all)
	{
		remove_all_aliases(&c->parser_context);
		return (0);
	}
	err = false;
	while (ft_argv_arg(&args, &arg))
		if (!sh_c_alias_unset(&c->parser_context, arg))
		{
			ft_dprintf(2, "unalias: %ts: Alias not found%n", arg);
			err = true;
		}
	return (err ? 1 : 0);
	(void)data;
}
