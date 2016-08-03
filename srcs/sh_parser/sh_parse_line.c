/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/30 23:26:24 by juloo             #+#    #+#             */
/*   Updated: 2016/08/04 01:15:12 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/lexer.h"
#include "p_sh_parser.h"

#define T(T, ...)	(&(t_sh_parse_token){SH_PARSE_T_##T, {__VA_ARGS__}})
#define SH(T)		SH_PARSE_T_##T

static t_lexer_def const	g_sh_lexer = LEXER_DEF(

	LEXER_STATE("sh-base-subst", (),
		LEXER_T("${", T(SUBST, .subst=SH(SUBST_EXPR))),
		LEXER_T("$(", T(SUBST, .subst=SH(SUBST_SUBSHELL))),
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
		LEXER_T(";", T(COMPOUND_END, .compound_end=SH(END_SEMICOLON))),
		LEXER_T("&", T(COMPOUND_END, .compound_end=SH(END_AMPERSAND))),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(END_NEWLINE))),

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

	LEXER_STATE("sh-string", ("sh-base-subst", "sh-base-text"),
		LEXER_T("\"", T(STRING, .string=SH(STRING_END)), .pop=true),
		LEXER_T("\\\"", T(ESCAPED)),
	),

	LEXER_STATE("sh-string-single", (),
		LEXER_T("\'", T(STRING, .string=SH(STRING_END)), .pop=true),
	),

	LEXER_STATE("sh-comment", (),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(END_NEWLINE)), .pop=true),
	),

);

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

static void		push_token(t_sh_text *text, t_sub str, t_sh_token t)
{
	ft_dstradd(&text->text, str);
	ft_vpush(&text->tokens, &t, 1);
}

static void		push_token_string(t_sh_text *text, t_sub str, bool escaped)
{
	t_sh_token		token;

	token = SH_TOKEN(STRING, .token_len=str.length);
	if (escaped)
		token.type |= SH_F_T_QUOTED;
	push_token(text, str, token);
}

static bool		sh_parse_simple_cmd(t_sh_parser *p, t_sh_simple *dst)
{
	t_sh_parse_token const	*t;

	dst->text = SH_TEXT();
	while (ft_lexer_next(&p->l))
	{
		if ((t = SH_T(p)) == NULL)
			push_token_string(&dst->text, p->l.t.token, false);
		else if (t->type == SH_PARSE_T_SPACE)
			push_token(&dst->text, SUB0(), SH_TOKEN(SPACE, 0));
		else if (t->type == SH_PARSE_T_SUBST)
			ASSERT(!"ignore token");
		else if (t->type == SH_PARSE_T_STRING)
			ASSERT(!"ignore token");
		else if (t->type == SH_PARSE_T_COMMENT)
			ASSERT(!"ignore token");
		else if (t->type == SH_PARSE_T_COMPOUND_END
			|| t->type == SH_PARSE_T_LIST_END
			|| t->type == SH_PARSE_T_PIPELINE_END)
		{
			if (dst->text.tokens.length == 0)
				return (ASSERT(!"Unexpected token"));
			break ;
		}
		else if (t->type == SH_PARSE_T_BACKSLASH)
			ASSERT(!"ignore token");
		else if (t->type == SH_PARSE_T_ESCAPED)
			push_token_string(&dst->text, SUB_FOR(p->l.t.token, 1), true);
		else if (t->type == SH_PARSE_T_REDIR)
			push_token(&dst->text, SUB0(), SH_TOKEN(REDIR, .redir_type=t->redir));
		else if (t->type == SH_PARSE_T_HEREDOC)
			ASSERT(!"ignore token");
	}
	return (true);
}

static bool		sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd)
{
	// TODO: if
	// TODO: for
	// TODO: while
	// TODO: until
	cmd->type = SH_CMD_SIMPLE;
	return (sh_parse_simple_cmd(p, &cmd->simple));
}

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
			return (ASSERT(!"Unexpected EOF"));
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
			return (ASSERT(!"Unexpected EOF"));
		dst->next = NEW(t_sh_list_next);
		dst->next->type = next_t;
		dst = &dst->next->next;
	}
	return (true);
}

static bool		sh_parse_compound(t_sh_parser *p, t_sh_compound *dst)
{
	while (true)
	{
		dst->flags = 0;
		dst->next = NULL;
		if (!sh_parse_list(p, &dst->list))
			return (false);
		if (p->l.eof
			|| !ASSERT(SH_T_EQU(p, COMPOUND_END))
			|| SH_T(p)->compound_end == SH_PARSE_T_END_NEWLINE)
			break ;
		if (SH_T(p)->compound_end == SH_PARSE_T_END_AMPERSAND)
			dst->flags |= SH_COMPOUND_ASYNC;
		if (!sh_ignore_spaces(p))
			break ;
		dst->next = NEW(t_sh_compound);
		dst = dst->next;
	}
	return (true);
}

bool			sh_parse_line(t_in *in, t_sh_compound *dst, t_sh_parse_err *err)
{
	static t_lexer_state	*lexer_state = NULL;
	t_sh_parser				p;
	bool					r;

	if (lexer_state == NULL)
		lexer_state = ft_lexer_build(&g_sh_lexer, SUBC("sh-compound"));
	p = (t_sh_parser){LEXER(in), err};
	ft_lexer_push(&p.l, lexer_state);
	r = (sh_ignore_spaces(&p) || ASSERT(!"Empty line"))
		&& sh_parse_compound(&p, dst);
	ft_lexer_destroy(&p.l, true);
	return (r);
}
