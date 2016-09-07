/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:50:51 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 17:36:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "p_default_builtins.h"

#define ECHO_OPT_N		(1 << 0)
#define ECHO_OPT_E		(1 << 1)

static bool		echo_parse_opt(t_sub arg, uint32_t *dst)
{
	uint32_t		i;
	uint32_t		flags;

	if (arg.length < 2 || arg.str[0] != '-')
		return (false);
	flags = *dst;
	i = 0;
	while (++i < arg.length)
		if (arg.str[i] == 'n')
			flags |= ECHO_OPT_N;
		else if (arg.str[i] == 'e')
			flags |= ECHO_OPT_E;
		else if (arg.str[i] == 'E')
			flags &= ~ECHO_OPT_E;
		else
			return (false);
	*dst = flags;
	return (true);
}

char const		g_echo_escaped[256] = {
	['a'] = '\a',
	['b'] = '\b',
	['E'] = '\033',
	['f'] = '\f',
	['n'] = '\n',
	['r'] = '\r',
	['t'] = '\t',
	['v'] = '\v',
	['\\'] = '\\',
	['0'] = '0',
};

static void		echo_print_escaped(char const *arg)
{
	uint32_t const	len = ft_strlen(arg);
	uint32_t		i;
	uint32_t		prev;

	i = 0;
	prev = 0;
	while (i < len)
	{
		while (i < len && arg[i] != '\\')
			i++;
		ft_printf("%ts", SUB(arg + prev, i - prev));
		if (i >= len)
			break ;
		i++;
		if (g_echo_escaped[(uint8_t)arg[i]] != 0)
		{
			ft_printf("%c", g_echo_escaped[(uint8_t)arg[i]]);
			prev = ++i;
		}
		else
		{
			prev++;
			i++;
		}
	}
}

static void		echo_print_normal(char const *arg)
{
	ft_printf("%s", arg);
}

int				sh_builtin_echo(t_sh_context *c, void *data, t_argv args)
{
	uint32_t		flags;
	uint32_t		i;
	void			(*print)(char const *arg);

	flags = 0;
	i = 1;
	while (i < args.argc && echo_parse_opt(ft_sub(args.argv[i], 0, -1),
			&flags))
		i++;
	print = (flags & ECHO_OPT_E) ? &echo_print_escaped : &echo_print_normal;
	if (i < args.argc)
		while (true)
		{
			print(args.argv[i]);
			i++;
			if (i >= args.argc)
				break ;
			ft_printf(" ");
		}
	ft_printf((flags & ECHO_OPT_N) ? "%!" : "%n");
	return (0);
	(void)c;
	(void)data;
}
