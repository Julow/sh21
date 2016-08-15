/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/30 23:26:24 by juloo             #+#    #+#             */
/*   Updated: 2016/08/16 00:11:43 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/lexer.h"
#include "p_sh_parser.h"

#include <stdlib.h>

#define T(T, ...)	(&SH_PARSE_T(T, ##__VA_ARGS__))

static t_lexer_def const	g_sh_lexer = LEXER_DEF(

	LEXER_STATE("sh-base-subst", (),
		LEXER_T("${", T(SUBST, .subst=SH(SUBST_EXPR))),
		LEXER_T("$(", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-subshell"),
		LEXER_T("$((", T(SUBST, .subst=SH(SUBST_MATH))),
		LEXER_T("`", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-backquote"),
		LEXER_T("$", T(SUBST, .subst=SH(SUBST_DOLLAR))),
		LEXER_T("\\`", T(ESCAPED)),
		LEXER_T("\\$", T(ESCAPED)),
	),

	LEXER_STATE("sh-base-text", ("sh-base-subst"),
		LEXER_T(" ", T(SPACE)),
		LEXER_T("\t", T(SPACE)),
		LEXER_T("\\ ", T(ESCAPED)),
		LEXER_T("\\\t", T(ESCAPED)),

		LEXER_T("\"", T(STRING, .string=SH(STRING_DOUBLE)), .push="sh-string"),
		LEXER_T("\\\"", T(ESCAPED)),
		LEXER_T("\'", T(STRING, .string=SH(STRING_SIMPLE)), .push="sh-string-single"),
		LEXER_T("\\'", T(ESCAPED)),

		LEXER_T("#", T(COMMENT), .push="sh-comment"),
		LEXER_T("\\#", T(ESCAPED)),

		LEXER_T("\\", T(BACKSLASH)),
		LEXER_T("\\\\", T(ESCAPED)),

		LEXER_T("", T(TEXT)),
	),

	LEXER_STATE("sh-base-cmd", ("sh-base-text"),
		LEXER_T("|", T(PIPELINE_END)),
		LEXER_T("&&", T(LIST_END, .list_end=SH_LIST_AND)),
		LEXER_T("||", T(LIST_END, .list_end=SH_LIST_OR)),
		LEXER_T(";", T(COMPOUND_END, .compound_end=SH(COMPOUND_SEMICOLON))),
		LEXER_T("&", T(COMPOUND_END, .compound_end=SH(COMPOUND_AMPERSAND))),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(COMPOUND_NEWLINE))),

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
		LEXER_T("\\>", T(ESCAPED)),
		LEXER_T("\\<", T(ESCAPED)),
	),

	LEXER_STATE("sh-compound", ("sh-base-cmd")),

	LEXER_STATE("sh-subst-subshell", ("sh-base-cmd"),
		LEXER_T(")", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL)), .pop=true),
	),

	LEXER_STATE("sh-subst-backquote", ("sh-base-cmd"),
		LEXER_T("\\`", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-backquote-rev"),
		LEXER_T("`", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL)), .pop=true),
	),

	LEXER_STATE("sh-subst-backquote-rev", ("sh-base-cmd"),
		LEXER_T("`", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-backquote"),
		LEXER_T("\\`", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL)), .pop=true),
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

bool			sh_parse(t_in *in, t_sh_compound *dst, t_sh_parse_err *err)
{
	static t_lexer_state	*lexer_state = NULL;
	t_sh_parser				p;
	bool					r;

	if (lexer_state == NULL)
		lexer_state = ft_lexer_build(&g_sh_lexer, SUBC("sh-compound"));
	p = (t_sh_parser){LEXER(in, lexer_state), err, false};
	sh_ignore_newlines(&p);
	r = sh_parse_compound(&p, dst, false)
		&& (p.l.eof || (SH_T_EQU(&p, COMPOUND_END)
				&& SH_T(&p)->compound_end == SH(COMPOUND_NEWLINE))
			|| (sh_destroy_compound(dst), sh_parse_error(&p, SH_E_UNEXPECTED)));

	if (!r)
		ft_printf("ERROR: %s '%ts'%n", ((char const*[]){
			[SH_E_ERROR] = "wtf",
			[SH_E_UNEXPECTED] = "unexpected token",
			[SH_E_EOF] = "unexpected end of file",
			[SH_E_UNCLOSED_STRING] = "unclosed string",
			[SH_E_UNCLOSED_SUBSHELL] = "unclosed subshell",
		})[err->err], p.l.t.token);


	ft_lexer_pop(&p.l, NULL);
	ft_lexer_destroy(&p.l);
	return (r);
}
