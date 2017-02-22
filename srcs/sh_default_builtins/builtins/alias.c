/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/18 11:50:50 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:38:07 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

static void		alias_print(t_sub name, t_sub val)
{
	uint32_t			i;
	uint32_t			tmp;

	ft_printf("alias %ts='", name);
	i = 0;
	while (true)
	{
		tmp = ft_subfind_c(val, '\'', i);
		ft_printf("%ts", SUB(val.str + i, tmp - i));
		i = tmp;
		if (i >= val.length)
			break ;
		ft_printf("'\\''");
		i++;
	}
	ft_printf("'%n");
}

static bool		print_alias(t_sh_parser_context const *c, t_sub name)
{
	t_sub const *const	alias = sh_c_alias_get(c, name);

	if (alias == NULL)
	{
		ft_dprintf(2, "alias: %ts: Alias not found%n", name);
		return (false);
	}
	alias_print(name, *alias);
	return (true);
}

static bool		set_alias(t_sh_parser_context *c, t_sub name, t_sub val)
{
	t_sub const		forbidden = SUBC(SH_ALIAS_NAME_FORBIDDEN);
	uint32_t		i;

	i = 0;
	while (i < name.length)
	{
		if (ft_subfind_c(forbidden, name.str[i], 0) < forbidden.length)
		{
			ft_dprintf(2, "alias: '%ts': Invalid alias name%n", name);
			return (false);
		}
		i++;
	}
	sh_c_alias_set(c, name, val);
	return (true);
}

static void		print_all_aliases(t_sh_parser_context const *c)
{
	t_sh_c_alias const	*alias = ft_set_cbegin(&c->aliases, NULL);

	while (alias != NULL)
	{
		alias_print(alias->name, alias->val);
		alias = ft_set_cnext(alias);
	}
}

int				sh_builtin_alias(t_sh_context *c, void *data, t_argv args)
{
	t_sub			arg;
	uint32_t		offset;
	bool			print;
	bool			err;

	print = (args.argc <= 1);
	while (ft_argv_opt(&args, &arg))
		if (SUB_EQU(arg, SUBC("p")))
			print = true;
		else
		{
			ft_dprintf(2, "alias: %ts: Unknown option%n", arg);
			return (2);
		}
	if (print)
		print_all_aliases(&c->parser_context);
	err = false;
	while (ft_argv_arg(&args, &arg))
		if ((offset = ft_subfind_c(arg, '=', 1)) >= arg.length)
			err |= !print_alias(&c->parser_context, arg);
		else
			err |= !set_alias(&c->parser_context,
				SUB_LEN(arg, offset), SUB_FOR(arg, offset + 1));
	return (err ? 1 : 0);
	(void)data;
}
