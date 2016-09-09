/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:39 by juloo             #+#    #+#             */
/*   Updated: 2016/09/09 18:47:12 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "p_sh_parser.h"

#include <stdlib.h>

static bool		sh_parse_simple_cmd(t_sh_parser *p, t_sh_cmd *dst)
{
	bool			r;

	dst->simple.text = SH_TEXT();
	r = true;
	while (!p->t.eof)
		if (SH_T(p)->type == SH(TEXT)
			&& sh_parse_redir_left(p, &dst->redirs, &r))
			;
		else if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			r = g_sh_parse_text[SH_T(p)->type](p, &dst->simple.text, false);
			if (!r || !ft_tokenize(&p->t))
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

	if (!ft_tokenize(&p->t))
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

static bool		sh_parse_function_def(t_sh_parser *p, t_sh_cmd *dst)
{
	t_sh_func_def	*func_def;

	while (true)
	{
		if (!ft_tokenize(&p->t))
			return (sh_parse_error(p, SH_E_EOF));
		if (SH_T(p)->type != SH(SPACE))
			break ;
	}
	if (SH_T(p)->type != SH_PARSE_T_TEXT)
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	if (!ft_subis_identifier(p->t.token_str))
		return (sh_parse_error(p, SH_E_INVALID_ID));
	func_def = MALLOC(sizeof(t_sh_func_def) + p->t.token_str.length);
	func_def->name = SUB_DST(ENDOF(func_def), p->t.token_str);
	sh_ignore_newlines(p);
	if (!sh_parse_cmd(p, &func_def->body))
	{
		free(func_def);
		return (false);
	}
	dst->function_def = func_def;
	return (true);
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
	{SUBC("function"), SH_CMD_FUNCTION_DEF},
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
	[SH_CMD_FUNCTION_DEF] = &sh_parse_function_def,
};

bool			sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd)
{
	uint32_t				i;

	cmd->type = SH_CMD_EMPTY;
	cmd->redirs = SH_REDIR_LST();
	if (!ft_tokenize(&p->t))
		return (true);
	cmd->type = SH_CMD_SIMPLE;
	if (SH_T(p)->type == SH(TEXT))
	{
		// HERE alias
		i = 0;
		while (i < ARRAY_LEN(g_clauses_begin))
			if (SUB_EQU(p->t.token_str, g_clauses_begin[i].name))
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
