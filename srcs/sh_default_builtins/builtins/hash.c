/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/12 19:39:31 by juloo             #+#    #+#             */
/*   Updated: 2016/09/12 21:07:16 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/argv.h"
#include "ft/ft_printf.h"

#include "p_default_builtins.h"

#define HASH_OPT_P		(1 << 0)
#define HASH_OPT_D		(1 << 1)
#define HASH_OPT_T		(1 << 2)

static void		hash_opt_r(t_sh_context *c)
{
	sh_c_path_clear(c);
}

static bool		hash_opt_p(t_sh_context *c, t_argv *args, uint32_t *dst)
{
	t_sub			path;
	t_sub			name;

	if (!ft_argv_arg(args, &path) || !ft_argv_arg(args, &name))
	{
		ft_dprintf(2, "hash: -p: Expect more argument%n");
		return (false);
	}
	*dst |= HASH_OPT_P;
	sh_c_path_set(c, name, path);
	return (true);
}

static bool		hash_parse_opt(t_sh_context *c, t_argv *args, uint32_t *dst)
{
	t_sub			opt;

	while (ft_argv_opt(args, &opt))
	{
		if (SUB_EQU(opt, SUBC("r")))
			hash_opt_r(c);
		else if (SUB_EQU(opt, SUBC("d")))
			*dst = (*dst & ~HASH_OPT_T) | HASH_OPT_D;
		else if (SUB_EQU(opt, SUBC("t")))
			*dst = (*dst & ~HASH_OPT_D) | HASH_OPT_T;
		else if (SUB_EQU(opt, SUBC("p")))
		{
			if (!hash_opt_p(c, args, dst))
				return (false);
		}
		else
		{
			ft_dprintf(2, "hash: %ts: Unknown option%n", opt);
			return (false);
		}
	}
	return (true);
}

static void		hash_print_all(t_sh_context const *c)
{
	t_sh_c_path const	*p;

	p = ft_set_cfirst(&c->path_cache, NULL);
	while (p != NULL)
	{
		ft_printf("%ts\t%ts%n", p->name, p->path);
		p = ft_set_cnext(p);
	}
}

static void		hash_print(t_sh_context *c, t_argv *args)
{
	t_sub			name;
	t_sub			path;

	while (ft_argv_arg(args, &name))
	{
		path = sh_c_path_get(c, name);
		if (path.length == 0)
			ft_dprintf(2, "hash: %ts: Not found%n", name);
		else
			ft_printf("%ts%n", path);
	}
}

static void		hash_delete(t_sh_context *c, t_argv *args)
{
	t_sub			name;

	while (ft_argv_arg(args, &name))
	{
		if (!sh_c_path_remove(c, name))
			ft_dprintf(2, "hash: %ts: Not found%n", name);
	}
}

static void		hash_search(t_sh_context *c, t_argv *args)
{
	t_sub			name;
	t_dstr			tmp;

	tmp = DSTR0();
	while (ft_argv_arg(args, &name))
	{
		tmp.length = 0;
		sh_c_path_search(c, name, &tmp);
	}
	ft_dstrclear(&tmp);
}

int				sh_builtin_hash(t_sh_context *c, void *data, t_argv args)
{
	uint32_t		opt;

	opt = 0;
	if (!hash_parse_opt(c, &args, &opt))
		return (1);
	if (ARGV_END(&args) && !(opt & HASH_OPT_P))
		hash_print_all(c);
	else if (opt & HASH_OPT_T)
		hash_print(c, &args);
	else if (opt & HASH_OPT_D)
		hash_delete(c, &args);
	else
		hash_search(c, &args);
	return (0);
	(void)data;
}
