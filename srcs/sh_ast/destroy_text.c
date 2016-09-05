/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/23 20:10:34 by juloo             #+#    #+#             */
/*   Updated: 2016/09/05 17:41:44 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/ast.h"
#include "p_destroy_cmd.h"

#include <stdlib.h>

static void		destroy_subst_param_str(t_sh_subst_param *subst_param)
{
	sh_destroy_text(&subst_param->str);
	free(subst_param);
}

static void		destroy_subst_param_end(t_sh_subst_param *subst_param)
{
	free(subst_param);
}

static void		destroy_subst_param_repl(t_sh_subst_param *subst_param)
{
	sh_destroy_text(&subst_param->repl[0]);
	sh_destroy_text(&subst_param->repl[1]);
	free(subst_param);
}

static void		(*g_sh_destroy_subst_param[])(t_sh_subst_param *param) = {
	[SH_SUBST_PARAM_DEF_NULL] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_DEF_UNSET] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_NULL] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_UNSET] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REPL_NULL] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REPL_UNSET] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_ERR_NULL] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_ERR_UNSET] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_UPPER_FIRST] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_UPPER] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_LOWER_FIRST] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_LOWER] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_INVCASE_FIRST] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_INVCASE] = &destroy_subst_param_end,
	[SH_SUBST_PARAM_REM_BEGIN] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REM_BEGIN_LONG] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REM_END] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REM_END_LONG] = &destroy_subst_param_str,
	[SH_SUBST_PARAM_REPL_FIRST] = &destroy_subst_param_repl,
	[SH_SUBST_PARAM_REPL_LAST] = &destroy_subst_param_repl,
	[SH_SUBST_PARAM_REPL_ALL] = &destroy_subst_param_repl,
};

static void		destroy_subshell(t_sh_compound *cmd)
{
	sh_destroy_compound(cmd);
	free(cmd);
}

void			sh_destroy_text(t_sh_text *text)
{
	uint32_t		i;
	t_sh_token		*t;

	i = 0;
	while (i < text->tokens.length)
	{
		t = VECTOR_GET(text->tokens, i++);
		if (t->type == SH_T_SUBSHELL)
			destroy_subshell(t->cmd);
		else if (t->type == SH_T_SUBST_PARAM)
			g_sh_destroy_subst_param[t->subst_param->type](t->subst_param);
	}
	ft_dstrclear(&text->text);
	ft_vclear(&text->tokens);
}
