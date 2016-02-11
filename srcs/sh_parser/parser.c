/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 18:31:08 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

/*
** Add a subst
** Does not init 'val' field
** 'range' is set to the 'length' last char of text
*/
t_sh_subst		*sh_simple_cmd_add_subst(t_sh_simple_cmd *s,
					enum e_sh_subst_type type, uint32_t length)
{
	t_sh_subst *const	subst = ft_vpush(&s->substs, NULL, 1);

	subst->type = type;
	subst->range = VEC2U(s->text.length - length, s->text.length);
	return (subst);
}

static void		sh_parse_simple_cmd(t_parse_data *p, t_sh_simple_cmd *cmd)
{
	while (parse_token(p))
		switch ((uintptr_t)p->token_data)
		{
		case 0:
			ft_dstradd(&cmd->text, p->token);
			break ;
		case SH_T_ESCAPED:
			ft_dstradd(&cmd->text, SUB_FOR(p->token, 1));
			break ;
		case SH_T_SPACE:
			if (cmd->arg_ranges.length == 0
				|| (t_vec2u const*)VECTOR_GET(cmd->arg_ranges, cmd->arg_ranges.length - 1)->y < cmd->text.length)
				*(t_vec2u*)ft_vpush(&cmd->arg_ranges, NULL, 1) = cmd->text.length;
			break ;
		case SH_T_SUBST_PARAM:
		case SH_T_SUBST_PARAM_SPECIAL:
			ft_dstradd(&cmd->text, SUB_FOR(p->token, 1));
			sh_simple_cmd_add_subst(cmd, SH_SUBST_PARAM, p->token.length - 1);
			break ;
		}
}

bool			sh_parse_cmd(t_parse_data *p)
{
	struct s_parse_sh_frame	c;

	p->frame->data = &c;
	c = (struct s_parse_sh_frame){NEW(t_sh_cmd), p->t.char_count};
	*c.cmd = C(t_sh_cmd, SH_CMD_SIMPLE, LIST(t_sh_redir), {.cmd = C(t_sh_simple_cmd,
		DSTR0(), VECTOR(t_vec2u), VECTOR(t_sh_subst))}, SH_NEXT_NEW, NULL);
	if (p->frame->prev == NULL)
		*(void**)p->env = c.cmd;
	else
		sh_simple_cmd_add_subst(&((t_sh_cmd*)p->frame->prev->data)->val.cmd, SH_SUBST_CMD, 0)->val.cmd = c.cmd;
	sh_parse_simple_cmd(p, &c.cmd->val.cmd);
	return (true);
}

bool			sh_parse_sub(t_parse_data *p)
{
	if (!sh_parse_cmd(p))
		return (false);
	if (p->eof)
		ft_printf("UNCLOSED SUB%n");
	return (true);
}

bool			sh_parse_string(t_parse_data *p)
{
	p->frame->data = p->frame->prev->data;
	sh_parse_simple_cmd(p, &((t_sh_cmd*)p->frame->prev->data)->val.cmd);
	if (p->eof)
		ft_printf("UNCLOSED STRING%n");
	return (true);
}
