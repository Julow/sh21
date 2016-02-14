/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 12:53:21 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

bool				sh_parse_frame_ignore(t_parse_data *p)
{
	p->frame->data = p->frame->prev->data;
	while (parse_token(p))
		;
	return (true);
}

static t_sh_token	*sh_put_token(t_sh_text *text, t_sh_token_t type)
{
	t_sh_token *const	token = ft_vpush(&text->tokens, NULL, 1);

	token->type = type;
	return (token);
}

static t_sh_token	*sh_put_t_string(t_sh_text *text, t_sub str, bool quoted)
{
	t_sh_token			*token;

	if (text->tokens.length == 0 || ((token = VECTOR_GET(text->tokens,
				text->tokens.length - 1))->type != SH_T_STRING
			&& token->type != SH_T_STRING_QUOTED))
	{
		token = ft_vpush(&text->tokens, NULL, 1);
		token->type = SH_T_STRING;
		token->val.token_len = str.length;
	}
	else
		token->val.token_len += str.length;
	ft_dstradd(&text->text, str);
	if (quoted)
		token->type = SH_T_STRING_QUOTED;
	return (token);
}

bool				sh_parse_text(t_parse_data *p, t_sh_text *text)
{
	t_sh_token			*last_token;

	last_token = (text->tokens.length == 0) ? NULL
		: VECTOR_GET(text->tokens, text->tokens.length - 1);
	while (parse_token(p))
		switch ((uintptr_t)p->token_data)
		{
		case SH_PARSE_T_SPACE:
			if (last_token == NULL || last_token->type == SH_T_SPACE)
				break ;
			last_token = sh_put_token(text, SH_T_SPACE);
			break ;
		case SH_PARSE_T_ESCAPED:
			last_token = sh_put_t_string(text, SUB_FOR(p->token, 1), true);
			break ;
		case SH_PARSE_T_NONE:
			last_token = sh_put_t_string(text, p->token, false);
			break ;
		case SH_PARSE_T_BACKSLASH:
			last_token = sh_put_t_string(text, SUB0(), true);
			break ;
		case SH_PARSE_T_PARAM:
		case SH_PARSE_T_PARAM_SPECIAL:
			last_token = sh_put_token(text, SH_T_PARAM);
			ft_dstradd(&text->text, SUB_FOR(p->token, 1));
			last_token->val.token_len = p->token.length - 1;
			break ;
		default:
			ASSERT(false, "Unexpected token");
			break ;
		}
	return (true);
}

static void			sh_put_cmd(t_parse_data *p, t_sh_cmd *cmd)
{
	if (p->frame->prev == NULL)
		p->env = cmd;
	else
		sh_put_token(&((t_sh_cmd*)p->frame->prev->data)->text,
			SH_T_SUBSHELL)->val.cmd = cmd;
}

static t_sh_cmd		*sh_parse_cmd(t_parse_data *p)
{
	t_sh_cmd *const	cmd = NEW(t_sh_cmd);

	*cmd = SH_CMD();
	p->frame->data = cmd;
	if (!sh_parse_text(p, &cmd->text))
		return (sh_destroy_cmd(cmd), NULL); // TODO: free
	if (((uintptr_t)p->token_data) == SH_PARSE_T_AND)
		cmd->next_type = SH_NEXT_AND;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_OR)
		cmd->next_type = SH_NEXT_OR;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_PIPE)
		cmd->next_type = SH_NEXT_PIPE;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_AMPERSAND)
		cmd->async = true;
	return (cmd);
}

bool				sh_parse_frame_cmd(t_parse_data *p)
{
	t_sh_cmd *const		cmd = sh_parse_cmd(p);

	if (cmd == NULL)
		return (false);
	sh_put_cmd(p, cmd);
	return (true);
}

bool				sh_parse_frame_sub(t_parse_data *p)
{
	t_sh_cmd			*cmd;
	t_sh_cmd			*tmp;

	cmd = NULL;
	while (true)
	{
		if ((tmp = sh_parse_cmd(p)) == NULL)
			return (false);
		if (cmd == NULL)
			sh_put_cmd(p, tmp);
		else
			cmd->next = tmp;
		cmd = tmp;
		if (p->eof)
			return (ASSERT(false, "Unclosed sub"));
		if ((uintptr_t)p->token_data == SH_PARSE_T_NONE)
			break ;
	}
	return (true);
}

bool				sh_parse_frame_string(t_parse_data *p)
{
	t_sh_text *const	text = &((t_sh_cmd*)p->frame->prev->data)->text;

	p->frame->data = p->frame->prev->data;
	sh_put_t_string(text, SUB0(), true);
	sh_parse_text(p, text);
	if (p->eof)
		return (ASSERT(false, "Unclosed string"));
	return (true);
}
