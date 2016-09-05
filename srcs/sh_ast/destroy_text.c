/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/23 20:10:34 by juloo             #+#    #+#             */
/*   Updated: 2016/09/05 11:02:26 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh/ast.h"
#include "p_destroy_cmd.h"

#include <stdlib.h>

static void		destroy_subst_param(t_sh_subst_param *subst_param)
{
	ASSERT(!"TODO: destroy SUBST_PARAM");
	free(subst_param);
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
			destroy_subshell(t->cmd);
		else if (t->type == SH_T_SUBST_PARAM)
			destroy_subst_param(t->subst_param);
	}
	ft_dstrclear(&text->text);
	ft_vclear(&text->tokens);
}
