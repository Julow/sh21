/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 17:54:15 by jaguillo         ###   ########.fr       */
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

bool			sh_parse_cmd(t_parse_data *p)
{
	struct s_parse_sh_frame	c;

	p->frame->data = &c;
	c = (struct s_parse_sh_frame){NEW(t_sh_cmd), p->t.char_count};
	*c.cmd = C(t_sh_cmd, SH_CMD_SIMPLE, LIST(t_sh_redir), {.cmd = C(t_sh_simple_cmd,
		DSTR0(), VECTOR(uint32_t), VECTOR(t_sh_subst))}, SH_NEXT_NEW, NULL);
	if (p->frame->prev == NULL)
		*(void**)p->env = c.cmd;
	else
		sh_simple_cmd_add_subst(&((t_sh_cmd*)p->frame->prev->data)->val.cmd, SH_SUBST_CMD, 0)->val.cmd = c.cmd;
	while (parse_token(p))
		switch ((uintptr_t)p->token_data)
		{
		case 0:
			ft_dstradd(&c.cmd->val.cmd.text, p->token);
			break ;
		case SH_T_ESCAPED:
			ft_dstradd(&c.cmd->val.cmd.text, SUB_FOR(p->token, 1));
			break ;
		case SH_T_SPACE:
			if (c.cmd->val.cmd.arg_stops.length == 0
				|| *(uint32_t*)VECTOR_GET(c.cmd->val.cmd.arg_stops, c.cmd->val.cmd.arg_stops.length - 1) < c.cmd->val.cmd.text.length)
				*(uint32_t*)ft_vpush(&c.cmd->val.cmd.arg_stops, NULL, 1) = c.cmd->val.cmd.text.length;
			break ;
		case SH_T_SUBST_PARAM:
		case SH_T_SUBST_PARAM_SPECIAL:
			ft_dstradd(&c.cmd->val.cmd.text, SUB_FOR(p->token, 1));
			sh_simple_cmd_add_subst(&c.cmd->val.cmd, SH_SUBST_PARAM, p->token.length - 1);
			break ;
		}
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

// same as sh_parse_cmd
bool			sh_parse_string(t_parse_data *p)
{
	t_sh_simple_cmd *const	cmd = &((t_sh_cmd*)p->frame->prev->data)->val.cmd;

	p->frame->data = p->frame->prev->data;
	while (parse_token(p))
		switch ((uintptr_t)p->token_data)
		{
		case 0:
			ft_dstradd(&cmd->text, p->token);
			break ;
		case SH_T_ESCAPED:
			ft_dstradd(&cmd->text, SUB_FOR(p->token, 1));
			break ;
		case SH_T_SUBST_PARAM:
		case SH_T_SUBST_PARAM_SPECIAL:
			ft_dstradd(&cmd->text, SUB_FOR(p->token, 1));
			sh_simple_cmd_add_subst(cmd, SH_SUBST_PARAM, p->token.length - 1);
			break ;
		}
	return (true);
}
