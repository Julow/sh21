/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:39 by juloo             #+#    #+#             */
/*   Updated: 2016/09/08 19:02:39 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_simple_cmd(t_sh_parser *p, t_sh_cmd *dst)
{
	bool			r;

	dst->simple.text = SH_TEXT();
	r = true;
	while (!p->l.eof)
		if (SH_T(p)->type == SH(TEXT)
			&& sh_parse_redir_left(p, &dst->redirs, &r))
			;
		else if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			r = g_sh_parse_text[SH_T(p)->type](p, &dst->simple.text, false);
			if (!r || !ft_lexer_next(&p->l))
				break ;
		}
		else if (SH_T(p)->type == SH_PARSE_T_REDIR)
			r = sh_parse_redir(p, &dst->redirs);
		else if (SH_T(p)->type == SH_PARSE_T_HEREDOC)
			r = HARD_ASSERT(!"TODO: heredoc");
		else
			break ;
	return (r);
}

static bool		sh_parse_subshell(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->subshell = NEW(t_sh_compound);
	sh_ignore_newlines(p);
	if (sh_parse_compound(p, dst->subshell, true))
	{
		if (SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_SUBSHELL))
		{
			if (sh_parse_trailing_redirs(p, &dst->redirs))
				return (true);
		}
		sh_destroy_compound(dst->subshell);
	}
	free(dst->subshell);
	return (false);
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

static bool		sh_parse_bracket_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	sh_ignore_newlines(p);
	dst->bracket_clause = NEW(t_sh_compound);
	if (sh_parse_compound(p, dst->bracket_clause, true))
	{
		if (SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_BRACKET))
		{
			if (sh_parse_trailing_redirs(p, &dst->redirs))
				return (true);
		}
		else
			sh_parse_error(p, SH_E_UNEXPECTED);
		sh_destroy_compound(dst->bracket_clause);
	}
	free(dst->bracket_clause);
	return (false);
}

static struct {
	t_sub			name;
	t_sh_cmd_t		type;
} const					g_clauses_begin[] = {
	{SUBC("if"), SH_CMD_IF_CLAUSE},
	{SUBC("for"), SH_CMD_FOR_CLAUSE},
	{SUBC("while"), SH_CMD_WHILE_CLAUSE},
	{SUBC("until"), SH_CMD_UNTIL_CLAUSE},
	{SUBC("time"), SH_CMD_TIME_CLAUSE},
	{SUBC("!"), SH_CMD_NOT_CLAUSE},
	{SUBC("{"), SH_CMD_BRACKET_CLAUSE},
};

static bool		(*const g_sh_parse_clauses[])(t_sh_parser *p, t_sh_cmd *dst) = {
	[SH_CMD_SIMPLE] = &sh_parse_simple_cmd,
	[SH_CMD_SUBSHELL] = &sh_parse_subshell,
	[SH_CMD_IF_CLAUSE] = &sh_parse_if_clause,
	[SH_CMD_FOR_CLAUSE] = &sh_parse_for_clause,
	[SH_CMD_WHILE_CLAUSE] = &sh_parse_while_clause,
	[SH_CMD_UNTIL_CLAUSE] = &sh_parse_while_clause,
	[SH_CMD_TIME_CLAUSE] = &sh_parse_rec_cmd,
	[SH_CMD_NOT_CLAUSE] = &sh_parse_rec_cmd,
	[SH_CMD_BRACKET_CLAUSE] = &sh_parse_bracket_clause,
};

bool			sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd)
{
	uint32_t				i;

	cmd->type = SH_CMD_EMPTY;
	cmd->redirs = SH_REDIR_LST();
	if (!ft_lexer_next(&p->l))
		return (true);
	cmd->type = SH_CMD_SIMPLE;
	if (SH_T(p)->type == SH(TEXT))
	{
		// HERE alias
		i = 0;
		while (i < ARRAY_LEN(g_clauses_begin))
			if (SUB_EQU(p->l.t.token_str, g_clauses_begin[i].name))
			{
				cmd->type = g_clauses_begin[i].type;
				break ;
			}
			else
				i++;
	}
	else if (SH_T(p)->type == SH(PARENTHESIS_OPEN))
		cmd->type = SH_CMD_SUBSHELL;
	return (g_sh_parse_clauses[cmd->type](p, cmd)
			|| (sh_destroy_redir_lst(&cmd->redirs), false));
}
