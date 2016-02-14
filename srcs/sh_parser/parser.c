/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 02:11:17 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

bool				sh_parse_ignore(t_parse_data *p)
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
			if (last_token->type == SH_T_SPACE)
				break ;
			last_token = sh_put_token(text, SH_T_SPACE);
			break ;
		case SH_PARSE_T_ESCAPED:
			last_token = sh_put_t_string(text, SUB_FOR(p->token, 1), true);
			break ;
		case SH_PARSE_T_NONE:
			last_token = sh_put_t_string(text, p->token, false);
			break ;
		case SH_PARSE_T_PARAM:
		case SH_PARSE_T_PARAM_SPECIAL:
			last_token = sh_put_token(text, SH_T_PARAM);
			ft_dstradd(&text->text, SUB_FOR(p->token, 1));
			last_token->val.token_len = p->token.length - 1;
			break ;
		}
	return (true);
}

bool				sh_parse_cmd(t_parse_data *p)
{
	t_sh_cmd *const	cmd = NEW(t_sh_cmd);

	*cmd = (t_sh_cmd){(t_sh_text){DSTR0(), VECTOR(t_sh_token)},
		false, SH_NEXT_NEW, NULL},
	p->frame->data = cmd;
	if (!sh_parse_text(p, &cmd->text))
		return (false); // TODO: free
	if (p->frame->prev == NULL)
		p->env = cmd;
	else
		sh_put_token(&((t_sh_cmd*)p->frame->prev->data)->text,
			SH_T_SUBSHELL)->val.cmd = cmd;
	if (((uintptr_t)p->token_data) == SH_PARSE_T_AND)
		cmd->next_type = SH_NEXT_AND;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_OR)
		cmd->next_type = SH_NEXT_OR;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_PIPE)
		cmd->next_type = SH_NEXT_PIPE;
	else if (((uintptr_t)p->token_data) == SH_PARSE_T_AMPERSAND)
		cmd->async = true;
	return (true);
}

bool				sh_parse_sub(t_parse_data *p)
{
	if (!sh_parse_cmd(p))
		return (false);
	if (p->eof)
		ASSERT(false, "Unclosed sub");
	return (true);
}

bool				sh_parse_string(t_parse_data *p)
{
	t_sh_text *const	text = &((t_sh_cmd*)p->frame->prev->data)->text;

	p->frame->data = p->frame->prev->data;
	sh_put_t_string(text, SUB0(), true);
	sh_parse_text(p, text);
	if (p->eof)
		ASSERT(false, "Unclosed string");
	return (true);
}
