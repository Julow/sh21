/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:39 by juloo             #+#    #+#             */
/*   Updated: 2016/08/15 16:12:12 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_redir_right(t_sh_parser *p, t_sh_text *dst)
{
	while (ft_lexer_next(&p->l) && SH_T(p)->type != SH(SPACE)
			&& g_sh_parse_text[SH_T(p)->type] != NULL)
	{
		if (!g_sh_parse_text[SH_T(p)->type](p, dst))
			return (false);
	}
	return (true);
}

static bool		sh_parse_redir(t_sh_parser *p, t_sh_redir_lst *dst,
					int32_t left_fd)
{
	t_sh_redir			*redir;
	uint32_t const		text_offset = dst->text.tokens.length;

	ASSERT(SH_T(p)->type == SH_PARSE_T_REDIR);
	redir = ft_vpush(&dst->redirs, NULL, 1);
	*redir = SH_REDIR(SH_T(p)->redir, left_fd);
	sh_ignore_spaces(p);
	if (!sh_parse_redir_right(p, &dst->text))
		return (false);
	redir->right_len = dst->text.tokens.length - text_offset;
	return (redir->right_len > 0 || sh_parse_error(p, SH_E_UNEXPECTED));
}

static uint32_t	redir_get_left(t_sh_text *text)
{
	int32_t				left_fd;
	t_sub				left_str;
	t_sh_token const	*last;

	if (text->tokens.length == 0)
		return (-1);
	last = VECTOR_LAST(text->tokens);
	if (last->type != SH_T_STRING
		|| (text->tokens.length > 1 && VGETC(t_sh_token, text->tokens,
				text->tokens.length - 2).type != SH_T_SPACE))
		return (-1);
	left_str = SUB_FOR(DSTR_SUB(text->text),
			text->text.length - last->token_len);
	if (ft_subto_int(left_str, &left_fd) != left_str.length
		|| left_fd < 0)
		return (-1);
	text->tokens.length--;
	text->text.length -= left_str.length;
	return (left_fd);
}

static bool		sh_parse_simple_cmd(t_sh_parser *p, t_sh_cmd *dst)
{
	bool			r;

	dst->simple.text = SH_TEXT();
	r = true;
	while (!p->l.eof)
		if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			r = g_sh_parse_text[SH_T(p)->type](p, &dst->simple.text);
			if (!r || !ft_lexer_next(&p->l))
				break ;
		}
		else if (SH_T(p)->type == SH_PARSE_T_REDIR)
			r = sh_parse_redir(p, &dst->redirs,
					redir_get_left(&dst->simple.text));
		else if (SH_T(p)->type == SH_PARSE_T_HEREDOC)
			r = HARD_ASSERT(!"TODO: heredoc");
		else
			break ;
	return (r);
}

static bool		sh_parse_rec_cmd(t_sh_parser *p, t_sh_cmd *dst)
{
	bool			r;

	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	if (SH_T(p)->type != SH_PARSE_T_SPACE)
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	if (!sh_ignore_spaces(p))
		return (sh_parse_error(p, SH_E_EOF));
	dst->rec = NEW(t_sh_cmd);
	if (!(r = sh_parse_cmd(p, dst->rec)))
		free(dst->rec);
	return (r);
}

bool			sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd)
{
	static struct {
		t_sub			name;
		t_sh_cmd_t		type;
		bool			(*f)(t_sh_parser *p, t_sh_cmd *dst);
	} const					clauses[] = {
		{SUBC("if"), SH_CMD_IF_CLAUSE, &sh_parse_if_clause},
		{SUBC("for"), SH_CMD_FOR_CLAUSE, &sh_parse_for_clause},
		{SUBC("while"), SH_CMD_WHILE_CLAUSE, &sh_parse_while_clause},
		{SUBC("until"), SH_CMD_UNTIL_CLAUSE, &sh_parse_while_clause},
		{SUBC("time"), SH_CMD_TIME_CLAUSE, &sh_parse_rec_cmd},
		{SUBC("!"), SH_CMD_NOT_CLAUSE, &sh_parse_rec_cmd},
	};
	uint32_t				i;

	cmd->redirs = SH_REDIR_LST();
	if (!ft_lexer_next(&p->l))
		return (true);
	i = 0;
	if (SH_T(p)->type == SH(TEXT))
	{
		// HERE alias
		while (i < ARRAY_LEN(clauses))
			if (SUB_EQU(p->l.t.token, clauses[i].name))
			{
				cmd->type = clauses[i].type;
				return (clauses[i].f(p, cmd));
			}
			else
				i++;
	}
	cmd->type = SH_CMD_SIMPLE;
	return (sh_parse_simple_cmd(p, cmd));
}
