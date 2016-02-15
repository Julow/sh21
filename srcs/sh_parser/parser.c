/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/15 18:02:14 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include <stdlib.h>

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
	t_sh_parser_data const	*token_data;

	p->frame->data = text;
	while (parse_token(p))
	{
		token_data = p->token_data;
		if (token_data == NULL)
			sh_put_t_string(text, p->token, false);
		else
			switch (token_data->t)
			{
			case SH_PARSE_T_SPACE:
				if (text->tokens.length == 0 || VGET(t_sh_token, text->tokens,
						text->tokens.length - 1).type == SH_T_SPACE)
					break ;
				sh_put_token(text, SH_T_SPACE);
				break ;
			case SH_PARSE_T_ESCAPED:
				sh_put_t_string(text, SUB_FOR(p->token, 1), true);
				break ;
			case SH_PARSE_T_NONE:
				break ;
			case SH_PARSE_T_BACKSLASH:
				sh_put_t_string(text, SUB0(), true);
				break ;
			case SH_PARSE_T_REDIR:
				sh_put_token(text, SH_T_REDIR)->val.redir_type =
					token_data->data;
				break ;
			case SH_PARSE_T_PARAM:
			case SH_PARSE_T_PARAM_SPECIAL:
				sh_put_token(text, SH_T_PARAM)->val.token_len =
					p->token.length - 1;
				ft_dstradd(&text->text, SUB_FOR(p->token, 1));
				break ;
			default:
				ASSERT(false, "Unexpected token");
				break ;
			}
	}
	return (true);
}

static void			sh_put_cmd(t_parse_data *p, t_sh_cmd *cmd)
{
	if (p->frame->prev == NULL)
		p->env = cmd;
	else
		sh_put_token((t_sh_text*)p->frame->prev->data,
			SH_T_SUBSHELL)->val.cmd = cmd;
}

static t_sh_cmd		*sh_parse_cmd(t_parse_data *p)
{
	t_sh_cmd *const			cmd = NEW(t_sh_cmd);
	t_sh_parser_data const	*token_data;

	*cmd = SH_CMD();
	if (!sh_parse_text(p, &cmd->text))
		return (sh_destroy_cmd(cmd), NULL);
	if ((token_data = p->token_data) == NULL)
		;
	else if (token_data->t == SH_PARSE_T_NEXT)
		cmd->next_type = token_data->data;
	else if (token_data->t == SH_PARSE_T_AMPERSAND)
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
		if (((t_sh_parser_data const*)p->token_data)->t == SH_PARSE_T_NONE)
			break ;
	}
	return (true);
}

bool				sh_parse_frame_string(t_parse_data *p)
{
	t_sh_text *const	text = (t_sh_text*)p->frame->prev->data;

	sh_put_t_string(text, SUB0(), true);
	sh_parse_text(p, text);
	if (p->eof)
		return (ASSERT(false, "Unclosed string")); // TODO: parse error
	return (true);
}

bool			sh_parse_frame_expr(t_parse_data *p)
{
	t_sh_text *const	parent = (t_sh_text*)p->frame->prev->data;
	t_sh_expr			*expr;

	p->frame->data = NULL;
	if (!parse_token(p))
		return (ASSERT(false, "Parameter expected")); // TODO: parse error
	expr = MALLOC(sizeof(t_sh_expr) + p->token.length);
	*expr = SH_EXPR(p->token.length);
	p->frame->data = expr;
	ft_memcpy(ENDOF(expr), p->token.str, p->token.length);
	if (parse_token(p))
		return (ASSERT(false, "Unexpected token")); // TODO: free expr
	if (p->eof)
		return (ASSERT(false, "Unexpected EOF")); // TODO: free expr
	if (expr->type == SH_EXPR_NONE)
	{
		ft_dstradd(&parent->text, SUB(ENDOF(expr), expr->param_len));
		sh_put_token(parent, SH_T_PARAM)->val.token_len = expr->param_len;
		return (true); // TODO: free expr
	}
	if (p->token_data == NULL) // TODO: error
		return (ASSERT(false, "Unexpected EOF")); // TODO: free expr
	sh_put_token(parent, SH_T_EXPR)->val.expr = expr;
	return (true);
}

bool			sh_parse_frame_expr_val(t_parse_data *p)
{
	t_sh_expr *const		expr = p->frame->prev->data;
	t_sh_parser_data const	*data;

	if (expr == NULL)
		return (false);
	if (p->eof)
		return (ASSERT(false, "Unexpected EOF"));
	data = p->token_data;
	ASSERT(data != NULL && data->t == SH_PARSE_T_EXPR);
	expr->type = data->data;
	sh_parse_text(p, &expr->text);
	return (true);
}
