/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 19:46:45 by jaguillo         ###   ########.fr       */
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

#define VGET(TYPE,V,I)		(*((TYPE*)VECTOR_GET(V, I)))
#define VGETC(TYPE,V,I)		(VGET(TYPE const, (V), (U)))

bool			sh_parse_ignore(t_parse_data *p)
{
	p->frame->data = p->frame->prev->data;
	while (parse_token(p))
		;
	return (true);
}

static void		sh_parse_simple_cmd(t_parse_data *p, t_sh_simple_cmd *cmd)
{
	while (parse_token(p))
		switch ((uintptr_t)p->token_data)
		{
		case SH_T_NONE:

			ft_dstradd(&cmd->text, p->token);
			break ;
		case SH_T_ESCAPED:

			ft_dstradd(&cmd->text, SUB_FOR(p->token, 1));
			break ;
		case SH_T_SPACE:

			if (cmd->arg_stops.length == 0
				|| VGET(uint32_t, cmd->arg_stops, cmd->arg_stops.length - 1) < cmd->text.length)
				ft_vpush(&cmd->arg_stops, &cmd->text.length, 1);
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
	*c.cmd = C(t_sh_cmd, SH_CMD_SIMPLE, LIST(t_sh_redir), false, {.cmd = C(t_sh_simple_cmd,
		DSTR0(), VECTOR(uint32_t), VECTOR(t_sh_subst))}, SH_NEXT_NEW, NULL);
	if (p->frame->prev == NULL)
		*(void**)p->env = c.cmd;
	else
		sh_simple_cmd_add_subst(&((struct s_parse_sh_frame*)p->frame->prev->data)->cmd->val.cmd, SH_SUBST_CMD, 0)->val.cmd = c.cmd;
	sh_parse_simple_cmd(p, &c.cmd->val.cmd);
	switch ((uintptr_t)p->token_data)
	{
	case SH_T_NONE:
	case SH_T_SEMICOLON:
		break ;
	case SH_T_AND:
		c.cmd->next_type = SH_NEXT_AND;
		break ;
	case SH_T_OR:
		c.cmd->next_type = SH_NEXT_OR;
		break ;
	case SH_T_PIPE:
		c.cmd->next_type = SH_NEXT_PIPE;
		break ;
	case SH_T_AMPERSAND:
		c.cmd->async = true;
		break ;
	default:
		ASSERT(false, "Invalid cmd end token");
		break ;
	}
	return (true);
}

bool			sh_parse_sub(t_parse_data *p)
{
	if (!sh_parse_cmd(p))
		return (false);
	if (p->eof)
		ASSERT(false, "Unclosed sub");
	return (true);
}

bool			sh_parse_string(t_parse_data *p)
{
	p->frame->data = p->frame->prev->data;
	sh_parse_simple_cmd(p, &((struct s_parse_sh_frame*)p->frame->data)->cmd->val.cmd);
	if (p->eof)
		ASSERT(false, "Unclosed string");
	return (true);
}
