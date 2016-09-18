/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 15:06:31 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/18 11:48:07 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "sh/context.h"

#include "p_sh_context.h"

static void		context_init_argv(t_sh_context *dst, char const *const *argv)
{
	uint32_t		i;

	ft_str_list_append(&dst->pos_params, SUB0());
	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i] != NULL)
	{
		ft_str_list_append(&dst->pos_params, ft_sub(argv[i], 0, -1));
		ft_str_list_break(&dst->pos_params);
		i++;
	}
}

static void		context_init_env(t_sh_context *dst, char const *const *env)
{
	uint32_t		i;
	t_sub			sub;
	uint32_t		key_len;

	if (env == NULL)
		return ;
	i = 0;
	while (env[i] != NULL)
	{
		sub = ft_sub(env[i], 0, -1);
		key_len = ft_subfind_c(sub, '=', 0);
		sh_c_var_set(dst, SUB_LEN(sub, key_len),
			(key_len == sub.length) ? SUB0() : SUB_FOR(sub, key_len + 1));
		sh_c_env_export(dst, SUB_LEN(sub, key_len), true);
		i++;
	}
}

static void		context_init_shlvl(t_sh_context *dst)
{
	t_sub const		val = sh_c_var_get(dst, SUBC("SHLVL"));
	uint32_t		shlvl;
	char			buff[16];
	uint32_t		length;

	if (val.length == 0 || ft_subto_uint(val, &shlvl) != val.length)
		shlvl = 0;
	length = ft_sprintf(buff, "%u", shlvl + 1);
	sh_c_var_set(dst, SUBC("SHLVL"), SUB(buff, length));
}

void			sh_context_init(t_sh_context *dst,
					char const *const *argv, char const *const *env)
{
	*dst = (t_sh_context){
		SET(sh_c_var_cmp, 0),
		STRSET(0),
		STR_LIST(),
		0,
		SET(sh_c_builtin_cmp, 0),
		SET(sh_c_function_cmp, 0),
		SET(sh_c_path_cmp, 0),
		SH_PARSER_CONTEXT(),
	};
	context_init_argv(dst, argv);
	context_init_env(dst, env);
	context_init_shlvl(dst);
}
