/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_frame_compound.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/29 00:57:35 by juloo             #+#    #+#             */
/*   Updated: 2016/07/29 02:07:25 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static void		push_token(t_sh_text *text, t_sub str, t_sh_token t, bool quoted)
{
	if (quoted)
		t.type |= SH_F_T_QUOTED;
	ft_dstradd(&text->text, str);
	ft_vpush(&text->tokens, &t, 1);
}

static void		push_str_token(t_sh_text *text, t_sub str, bool quoted)
{
	push_token(text, str, SH_TOKEN(STRING, .token_len=str.length), quoted);
}

static bool		sh_parse_text(t_parse_data *p, t_sh_text *text)
{
	t_sh_token_data const	*t;

	*text = SH_TEXT();
	while (ft_parse_token(p, text))
	{
		if ((t = p->token_data) == NULL)
			push_str_token(text, p->token, false);
		else
			switch (t->type)
			{
			case SH_PARSE_T_NONE:
				break ;
			case SH_PARSE_T_SPACE:
				push_token(text, SUB0(), SH_TOKEN(SPACE, 0), false);
				break ;
			case SH_PARSE_T_END:
				ASSERT(!"Should not be reach");
				break ;
			case SH_PARSE_T_ESCAPED:
				push_str_token(text, SUB_FOR(p->token, 1), true);
				break ;

			case SH_PARSE_T_BACKSLASH:
				push_str_token(text, SUB0(), true);
				break ;
			case SH_PARSE_T_REDIR:
				push_token(text, SUB0(), SH_TOKEN(REDIR, .redir_type=t->redir), false);
				break ;
			case SH_PARSE_T_HEREDOC:
				push_token(text, SUB0(), SH_TOKEN(HEREDOC, .heredoc=NULL), false);
				break ;
			}
	}
	return (true);
}

static bool		sh_parse_cmd(t_parse_data *p, t_sh_cmd *cmd)
{
	cmd->type = SH_CMD_SIMPLE;
	return (sh_parse_text(p, &cmd->cmd.simple.text));
}

static bool		sh_parse_pipeline(t_parse_data *p, t_sh_pipeline *dst)
{
	t_sh_token_data const	*t;

	if (!sh_parse_cmd(p, &dst->cmd))
		return (false);
	dst->next = NULL;
	t = p->token_data;
	if (t != NULL && t->type == SH_PARSE_T_END
		&& t->end == SH_PARSE_T_END_PIPE)
	{
		dst->next = NEW(t_sh_pipeline);
		return (sh_parse_pipeline(p, dst->next) || (free(dst->next), false));
	}
	return (true);
}

static bool		sh_parse_list(t_parse_data *p, t_sh_list *dst)
{
	t_sh_token_data const	*t;

	if (!sh_parse_pipeline(p, &dst->pipeline))
		return (false);
	dst->next = NULL;
	t = p->token_data;
	if (t != NULL && t->type == SH_PARSE_T_END
		&& (t->end == SH_PARSE_T_END_AND
			|| t->end == SH_PARSE_T_END_OR))
	{
		dst->next = NEW(t_sh_list_next);
		dst->next->type = (t->end == SH_PARSE_T_END_AND) ?
			SH_LIST_AND : SH_LIST_OR;
		return (sh_parse_list(p, &dst->next->next) || (free(dst->next), false));
	}
	return (true);
}

bool			sh_parse_frame_compound(t_parse_data *p,
					void const *data, t_sh_compound *dst)
{
	t_sh_token_data const	*t;

	if (!sh_parse_list(p, &dst->list))
		return (false);
	dst->flags = 0;
	dst->next = NULL;
	t = p->token_data;
	if (t != NULL && t->type == SH_PARSE_T_END
		&& (t->end == SH_PARSE_T_END_AMPERSAND
			|| t->end == SH_PARSE_T_END_SEMICOLON))
	{
		if (t->end == SH_PARSE_T_END_AMPERSAND)
			dst->flags |= SH_COMPOUND_ASYNC;
		dst->next = NEW(t_sh_compound);
		return (sh_parse_frame_compound(p, NULL, dst->next) || (free(dst->next), false));
	}
	return (true);
	(void)data;
}
