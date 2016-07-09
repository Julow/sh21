/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:26:42 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/09 19:01:57 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h" // TODO: move ft_{a,s,as,f}printf to ft::out || move out ft_{,d}printf to it's own module
#include "internal.h"

#include <stdlib.h>

static bool			sh_parse_error(t_parse_data *p,
						t_sh_parse_err_t err, t_sub str)
{
	t_sh_parse_err		*e;

	ASSERT(!PARSE_ERROR(p));
	e = ((t_sh_parse_data*)p)->err;
	*e = (t_sh_parse_err){err, DSTR0()};
	if (err == SH_E_UNEXPECTED_TOKEN)
		ft_asprintf(&e->str, "Unexpected token '%ts'", str);
	else if (err == SH_E_UNEXPECTED_EOF)
		ft_asprintf(&e->str, "Unexpected end of file%ts", str);
	else if (err == SH_E_UNCLOSED_STRING || err == SH_E_UNCLOSED_SUBSHELL)
		ft_asprintf(&e->str, "Unexpected eof while looking for matching '%ts'", str);
	return (ft_parse_error(p));
}

bool				sh_parse_frame_ignore(t_parse_data *p)
{
	p->frame->data = p->frame->prev->data;
	while (ft_parse_token(p))
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

	token = NULL;
	if (text->tokens.length == 0 || ((token = VECTOR_GET(text->tokens,
			text->tokens.length - 1))->type & ~SH_F_T_QUOTED) != SH_T_STRING)
	{
		if (token != NULL && (token->type == SH_T_PARAM
			|| token->type == SH_T_EXPR || token->type == SH_T_SUBSHELL))
			quoted = true;
		token = ft_vpush(&text->tokens, NULL, 1);
		token->type = SH_T_STRING;
		token->val.token_len = str.length;
	}
	else
		token->val.token_len += str.length;
	ft_dstradd(&text->text, str);
	if (quoted)
		token->type |= SH_F_T_QUOTED;
	return (token);
}

bool				sh_parse_util_text(t_parse_data *p, t_sh_text *text)
{
	t_sh_parser_data const	*token_data;

	p->frame->data = text;
	while (ft_parse_token(p))
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
	return (!PARSE_ERROR(p));
}

static void			sh_put_cmd(t_parse_data *p, t_sh_cmd *cmd)
{
	t_sh_token			*t;

	if (p->frame->prev == NULL)
		((t_sh_parse_data*)p)->cmd = cmd;
	else
	{
		t = sh_put_token((t_sh_text*)p->frame->prev->data, SH_T_SUBSHELL);
		t->val.cmd = cmd;
	}
}

static bool			sh_parse_util_cmd(t_parse_data *p, bool compound)
{
	t_sh_cmd				*cmd;
	t_sh_cmd				**tmp;
	t_sh_parser_data const	*token_data;

	tmp = &cmd;
	while (true)
	{
		*tmp = NEW(t_sh_cmd);
		**tmp = SH_CMD();
		if (!sh_parse_util_text(p, &(*tmp)->text))
			return (sh_destroy_cmd(cmd), false);
		if ((token_data = p->token_data) != NULL
			&& token_data->t == SH_PARSE_T_NEXT)
		{
			(*tmp)->next_type = token_data->data;
			tmp = &(*tmp)->next;
			if (compound)
				continue ;
		}
		break ;
	}
	sh_put_cmd(p, cmd);
	return (true);
}

bool				sh_parse_frame_cmd(t_parse_data *p)
{
	return (sh_parse_util_cmd(p, false));
}

bool				sh_parse_frame_cmd_subshell(t_parse_data *p)
{
	if (!sh_parse_util_cmd(p, true))
		return (false);
	if (PARSE_EOF(p))
		return (sh_parse_error(p, SH_E_UNCLOSED_SUBSHELL, SUBC(")")));
	return (true);
}

bool				sh_parse_frame_string(t_parse_data *p)
{
	t_sh_text *const	text = (t_sh_text*)p->frame->prev->data;

	sh_put_t_string(text, SUB0(), true);
	if (!sh_parse_util_text(p, text))
		return (false);
	if (PARSE_EOF(p))
		return (sh_parse_error(p, SH_E_UNCLOSED_STRING, SUBC("\""))); // TODO: check if "'"
	return (true);
}

bool			sh_parse_frame_expr(t_parse_data *p)
{
	t_sh_text *const	parent = (t_sh_text*)p->frame->prev->data;
	t_sh_expr			*expr;

	p->frame->data = NULL;
	if (!ft_parse_token(p))
		return (sh_parse_error(p, SH_E_UNEXPECTED_TOKEN, p->token));
	expr = MALLOC(sizeof(t_sh_expr) + p->token.length);
	*expr = SH_EXPR(p->token.length);
	p->frame->data = expr;
	ft_memcpy(ENDOF(expr), p->token.str, p->token.length);
	if (ft_parse_token(p))
		return (sh_destroy_expr(expr), sh_parse_error(p, SH_E_UNEXPECTED_TOKEN, p->token));
	if (PARSE_EOF(p))
		return (sh_destroy_expr(expr), sh_parse_error(p, SH_E_UNEXPECTED_EOF, SUB0()));
	if (expr->type == SH_EXPR_NONE)
	{
		ft_dstradd(&parent->text, SUB(ENDOF(expr), expr->param_len));
		sh_put_token(parent, SH_T_PARAM)->val.token_len = expr->param_len;
		sh_destroy_expr(expr);
		return (true);
	}
	sh_put_token(parent, SH_T_EXPR)->val.expr = expr;
	return (true);
}

bool			sh_parse_frame_expr_val(t_parse_data *p)
{
	t_sh_expr *const		expr = p->frame->prev->data;
	t_sh_parser_data const	*data;

	if (expr == NULL)
		return (false);
	if (PARSE_EOF(p))
		return (sh_parse_error(p, SH_E_UNEXPECTED_EOF, SUB0()));
	data = p->token_data;
	ASSERT(data != NULL && data->t == SH_PARSE_T_EXPR);
	expr->type = data->data;
	return (sh_parse_util_text(p, &expr->text));
}
