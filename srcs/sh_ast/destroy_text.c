/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/23 20:10:34 by juloo             #+#    #+#             */
/*   Updated: 2016/07/28 00:12:03 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/ast.h"
#include "p_destroy_cmd.h"

static void		destroy_expr(t_sh_expr *expr)
{
	sh_destroy_text(&expr->text);
	free(expr);
}

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
			sh_destroy_compound(t->val.cmd);
		else if (t->type == SH_T_EXPR)
			destroy_expr(t->val.expr);
	}
	ft_dstrclear(&text->text);
	ft_vclear(&text->tokens);
}
