/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/30 23:26:24 by juloo             #+#    #+#             */
/*   Updated: 2016/08/05 23:00:26 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/lexer.h"
#include "p_sh_parser.h"

#include <stdlib.h>

#define T(T, ...)	(&(t_sh_parse_token){SH_PARSE_T_##T, {__VA_ARGS__}})
#define SH(T)		SH_PARSE_T_##T

static t_lexer_def const	g_sh_lexer = LEXER_DEF(

	LEXER_STATE("sh-base-subst", (),
		LEXER_T("${", T(SUBST, .subst=SH(SUBST_EXPR))),
		LEXER_T("$(", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-subshell"),
		LEXER_T("$((", T(SUBST, .subst=SH(SUBST_MATH))),
		LEXER_T("`", T(SUBST, .subst=SH(SUBST_BACKQUOTE))),
		LEXER_T("$", T(SUBST, .subst=SH(SUBST_DOLLAR))),
		LEXER_T("\\`", T(ESCAPED)),
		LEXER_T("\\$", T(ESCAPED)),
	),

	LEXER_STATE("sh-base-text", (),
		LEXER_T("\"", T(STRING, .string=SH(STRING_DOUBLE)), .push="sh-string"),
		LEXER_T("\'", T(STRING, .string=SH(STRING_SIMPLE)), .push="sh-string-single"),
		LEXER_T("\\", T(BACKSLASH)),
		LEXER_T("\\\"", T(ESCAPED)),
		LEXER_T("\\'", T(ESCAPED)),
		LEXER_T("\\\\", T(ESCAPED)),
	),

	LEXER_STATE("sh-base-cmd", ("sh-base-text", "sh-base-subst"),
		LEXER_T(" ", T(SPACE)),
		LEXER_T("\t", T(SPACE)),
		LEXER_T("\\ ", T(ESCAPED)),
		LEXER_T("\\\t", T(ESCAPED)),

		LEXER_T("#", T(COMMENT), .push="sh-comment"),
		LEXER_T("\\#", T(ESCAPED)),

		LEXER_T("|", T(PIPELINE_END)),
		LEXER_T("&&", T(LIST_END, .list_end=SH_LIST_AND)),
		LEXER_T("||", T(LIST_END, .list_end=SH_LIST_OR)),
		LEXER_T(";", T(COMPOUND_END, .compound_end=0)),
		LEXER_T("&", T(COMPOUND_END, .compound_end=SH(COMPOUND_ASYNC))),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(COMPOUND_STOP))),

		LEXER_T(">", T(REDIR, .redir=SH_REDIR_OUTPUT)),
		LEXER_T(">|", T(REDIR, .redir=SH_REDIR_OUTPUT_CLOBBER)),
		LEXER_T(">&", T(REDIR, .redir=SH_REDIR_OUTPUT_FD)),
		LEXER_T(">>", T(REDIR, .redir=SH_REDIR_APPEND)),
		LEXER_T("<", T(REDIR, .redir=SH_REDIR_INPUT)),
		LEXER_T("<&", T(REDIR, .redir=SH_REDIR_INPUT_FD)),
		LEXER_T("<>", T(REDIR, .redir=SH_REDIR_OPEN)),
		LEXER_T("<<", T(HEREDOC)),

		LEXER_T("\\;", T(ESCAPED)),
		LEXER_T("\\|", T(ESCAPED)),
		LEXER_T("\\&", T(ESCAPED)),
		LEXER_T("\\\n", T(ESCAPED)),
		LEXER_T("\\>", T(ESCAPED)),
		LEXER_T("\\<", T(ESCAPED)),
	),

	LEXER_STATE("sh-compound", ("sh-base-cmd")),

	LEXER_STATE("sh-subst-subshell", ("sh-base-cmd"),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=0)),
		LEXER_T(")", T(COMPOUND_END, .compound_end=SH(COMPOUND_STOP)), .pop=true),
	),

	LEXER_STATE("sh-string", ("sh-base-subst"),
		LEXER_T("\"", T(STRING, .string=SH(STRING_END)), .pop=true),
		LEXER_T("\\\"", T(ESCAPED)),
	),

	LEXER_STATE("sh-string-single", (),
		LEXER_T("\'", T(STRING, .string=SH(STRING_END)), .pop=true),
	),

	LEXER_STATE("sh-comment", (),
		LEXER_T("\n", T(COMMENT), .pop=true),
	),

);

/*
** ========================================================================== **
** Parse utils
*/

static bool		sh_ignore_spaces(t_sh_parser *p)
{
	t_sh_parse_token const	*t;

	while (ft_lexer_ahead(&p->l, NULL, V(&t)))
	{
		if (t == NULL || t->type != SH_PARSE_T_SPACE)
			return (true);
		if (!ft_lexer_next(&p->l)
			|| (t = SH_T(p)) == NULL || t->type != SH_PARSE_T_SPACE)
			ASSERT(!"Lexer ahead/next mismatch"); // TODO: remove
	}
	return (false);
}

static bool		sh_parse_error(t_sh_parser *p, t_sh_parse_err_t t)
{
	if (ASSERT(!p->error_set, "Double error") && p->err != NULL)
		p->err->err = t;
	p->error_set = true;
	return (false);
}

/*
** ========================================================================== **
** Parse text
*/

static void		push_token(t_sh_text *text, t_sub str, t_sh_token t, bool quoted)
{
	if (quoted)
		t.type |= SH_F_T_QUOTED;
	ft_dstradd(&text->text, str);
	ft_vpush(&text->tokens, &t, 1);
}

static void		push_token_string(t_sh_text *text, t_sub str, bool quoted)
{
	t_sh_token		token;

	token = SH_TOKEN(STRING, .token_len=str.length);
	push_token(text, str, token, quoted);
}

static bool		sh_parse_text_subst(t_sh_parser *p, t_sh_text *dst, bool quoted)
{
	t_sh_compound *const	cmd = NEW(t_sh_compound);
	bool					r;

	ft_lexer_push(&p->l);
	if ((r = sh_parse_compound(p, cmd)
			&& (!p->l.eof || sh_parse_error(p, SH_E_EOF))
			&& (SH_T(p)->type == SH_PARSE_T_COMPOUND_END || sh_parse_error(p, SH_E_ERROR))
			&& (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_STOP)
		))
		push_token(dst, SUB0(), SH_TOKEN(SUBSHELL, .cmd=cmd), quoted);
	else
		free(cmd);
	ft_lexer_pop(&p->l);
	return (r);
}

static bool		sh_parse_text_string(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;
	bool					r;

	ft_lexer_push(&p->l);
	r = true;
	while (ft_lexer_next(&p->l))
	{
		if ((t = SH_T(p)) == NULL)
			push_token_string(dst, p->l.t.token, true);
		else if (t->type == SH_PARSE_T_SUBST)
			r = sh_parse_text_subst(p, dst, true);
		else if (t->type == SH_PARSE_T_ESCAPED)
			push_token_string(dst, SUB_FOR(p->l.t.token, 1), true);
		else if (t->type == SH_PARSE_T_STRING)
		{
			ASSERT(t->string == SH_PARSE_T_STRING_END);
			break ;
		}
		else
			ASSERT(false, "Invalid token");
		if (!r)
			return (false);
	}
	ft_lexer_pop(&p->l);
	return (true);
}

static bool		sh_parse_text_comment(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;

	ft_lexer_push(&p->l);
	while (true)
	{
		if (!ft_lexer_ahead(&p->l, NULL, V(&t))
			|| (t != NULL && t->type == SH_PARSE_T_COMMENT))
			break ;
		if (!ft_lexer_next(&p->l))
			ASSERT(false);
	}
	ft_lexer_pop(&p->l);
	return (true);
}

static bool		sh_parse_text_heredoc(t_sh_parser *p, t_sh_text *dst)
{
	ASSERT(false);
	return (false);
}

static bool		sh_parse_text(t_sh_parser *p, t_sh_text *dst)
{
	t_sh_parse_token const	*t;
	bool					r;

	*dst = SH_TEXT();
	r = true;
	while (ft_lexer_next(&p->l))
	{
		if ((t = SH_T(p)) == NULL)
			push_token_string(dst, p->l.t.token, false);
		else if (t->type == SH_PARSE_T_SPACE)
			push_token(dst, SUB0(), SH_TOKEN(SPACE, 0), false);
		else if (t->type == SH_PARSE_T_SUBST)
			r = sh_parse_text_subst(p, dst, false);
		else if (t->type == SH_PARSE_T_STRING)
			r = sh_parse_text_string(p, dst);
		else if (t->type == SH_PARSE_T_COMMENT)
			r = sh_parse_text_comment(p, dst);
		else if (t->type == SH_PARSE_T_COMPOUND_END
			|| t->type == SH_PARSE_T_LIST_END
			|| t->type == SH_PARSE_T_PIPELINE_END)
			break ;
		else if (t->type == SH_PARSE_T_BACKSLASH)
			push_token_string(dst, SUB0(), true);
		else if (t->type == SH_PARSE_T_ESCAPED)
			push_token_string(dst, SUB_FOR(p->l.t.token, 1), true);
		else if (t->type == SH_PARSE_T_REDIR)
			push_token(dst, SUB0(), SH_TOKEN(REDIR, .redir_type=t->redir), false);
		else if (t->type == SH_PARSE_T_HEREDOC)
			r = sh_parse_text_heredoc(p, dst);
		else
			ASSERT(false, "Invalid token");
		if (!r)
			return (false);
	}
	return (true);
}

/*
** ========================================================================== **
** Cmd
*/

static bool		sh_parse_simple_cmd(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->type = SH_CMD_SIMPLE;
	return (sh_parse_text(p, &dst->simple.text));
}

static bool		sh_parse_if_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->type = SH_CMD_IF_CLAUSE;
	if (!ft_lexer_next(&p->l) || !sh_ignore_spaces(p))
		return (sh_parse_error(p, SH_E_EOF));
	ASSERT(!"if not implemented yet");
	return (false);
}

static bool		sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->type = SH_CMD_FOR_CLAUSE;
	ASSERT(!"for not implemented yet");
	return (false);
}

static bool		sh_parse_while_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->type = SH_CMD_WHILE_CLAUSE;
	ASSERT(!"while not implemented yet");
	return (false);
}

static bool		sh_parse_until_clause(t_sh_parser *p, t_sh_cmd *dst)
{
	dst->type = SH_CMD_UNTIL_CLAUSE;
	ASSERT(!"until not implemented yet");
	return (false);
}

static bool		sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd)
{
	t_sh_parse_token const	*t;
	t_sub					token_str;

	if (!ft_lexer_ahead(&p->l, &token_str, V(&t)))
		return (sh_parse_error(p, SH_E_EOF));
	if (t == NULL)
	{
		// HERE alias
		if (SUB_EQU(token_str, SUBC("if")))
			return (sh_parse_if_clause(p, cmd));
		else if (SUB_EQU(token_str, SUBC("for")))
			return (sh_parse_for_clause(p, cmd));
		else if (SUB_EQU(token_str, SUBC("while")))
			return (sh_parse_while_clause(p, cmd));
		else if (SUB_EQU(token_str, SUBC("until")))
			return (sh_parse_until_clause(p, cmd));
	}
	return (sh_parse_simple_cmd(p, cmd));
}

/*
** ========================================================================== **
** Cmd list
*/

static bool		sh_parse_pipeline(t_sh_parser *p, t_sh_pipeline *dst)
{
	while (true)
	{
		dst->next = NULL;
		if (!sh_parse_cmd(p, &dst->cmd))
			return (false);
		if (p->l.eof || !SH_T_EQU(p, PIPELINE_END))
			break ;
		if (!sh_ignore_spaces(p))
			return (sh_parse_error(p, SH_E_EOF));
		dst->next = NEW(t_sh_pipeline);
		dst = dst->next;
	}
	return (true);
}

static bool		sh_parse_list(t_sh_parser *p, t_sh_list *dst)
{
	t_sh_list_next_t	next_t;

	while (true)
	{
		dst->next = NULL;
		if (!sh_parse_pipeline(p, &dst->pipeline))
			return (false);
		if (p->l.eof || !SH_T_EQU(p, LIST_END))
			break ;
		next_t = SH_T(p)->list_end;
		if (!sh_ignore_spaces(p))
			return (sh_parse_error(p, SH_E_EOF));
		dst->next = NEW(t_sh_list_next);
		dst->next->type = next_t;
		dst = &dst->next->next;
	}
	return (true);
}

bool			sh_parse_compound(t_sh_parser *p, t_sh_compound *dst)
{
	while (true)
	{
		dst->flags = 0;
		dst->next = NULL;
		if (!sh_parse_list(p, &dst->list))
			return (false);
		if (p->l.eof || !ASSERT(SH_T_EQU(p, COMPOUND_END)))
			break ;
		if (SH_T(p)->compound_end & SH_PARSE_T_COMPOUND_ASYNC)
			dst->flags |= SH_COMPOUND_ASYNC;
		if (SH_T(p)->compound_end & SH_PARSE_T_COMPOUND_STOP)
			break ;
		if (!sh_ignore_spaces(p))
			break ;
		dst->next = NEW(t_sh_compound);
		dst = dst->next;
	}
	return (true);
}

/*
** ========================================================================== **
*/

bool			sh_parse(t_in *in, t_sh_compound *dst, t_sh_parse_err *err)
{
	static t_lexer_state	*lexer_state = NULL;
	t_sh_parser				p;
	bool					r;

	if (lexer_state == NULL)
		lexer_state = ft_lexer_build(&g_sh_lexer, SUBC("sh-compound"));
	ft_lexer_init(in, lexer_state, &p.l);
	p.err = err;
	r = (sh_ignore_spaces(&p) || ASSERT(!"Empty line"))
		&& sh_parse_compound(&p, dst);
	ft_lexer_destroy(&p.l, true);
	return (r);
}
