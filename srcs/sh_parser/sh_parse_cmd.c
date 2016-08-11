/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:39 by juloo             #+#    #+#             */
/*   Updated: 2016/08/11 11:47:03 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	ASSERT(!"for not implemented yet");
	return (false);
	(void)p;
	(void)dst;
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
	t_sh_parse_token const	*t;
	t_sub					token_str;
	uint32_t				i;

	if (!ft_lexer_ahead(&p->l, &token_str, V(&t)))
		return (sh_parse_error(p, SH_E_EOF));
	i = 0;
	if (t == NULL)
	{
		// HERE alias
		while (i < ARRAY_LEN(clauses))
			if (SUB_EQU(token_str, clauses[i].name))
			{
				if (!ft_lexer_next(&p->l))
					ASSERT(false);
				cmd->type = clauses[i].type;
				return (clauses[i].f(p, cmd));
			}
			else
				i++;
	}
	cmd->type = SH_CMD_SIMPLE;
	return (sh_parse_text(p, &cmd->simple.text));
}
