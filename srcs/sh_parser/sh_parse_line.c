/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/30 23:26:24 by juloo             #+#    #+#             */
/*   Updated: 2016/09/08 19:23:00 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/lexer.h"
#include "p_sh_parser.h"

#include <stdlib.h>

t_lexer_def const			g_sh_lexer_base = LEXER_DEF(
	(), NULL,

	LEXER_STATE("sh-base", (),
		LEXER_T("", T(TEXT)),
	),

	LEXER_STATE("sh-base-subst", ("sh-base"),
		LEXER_T("${", T(SUBST, .subst=SH(SUBST_PARAM))),
		LEXER_T("$((", T(SUBST, .subst=SH(SUBST_MATH))),
		LEXER_T("$(", T(SUBSHELL, .subst=SH(SUBSHELL_BRACE))),
		LEXER_T("`", T(SUBSHELL, .subst=SH(SUBSHELL_BACKQUOTE))),
		LEXER_T("$\0000-9\0", T(PARAM_POS, .param_prefix=1)),
		LEXER_T("$\0a-zA-Z_\0\0+a-zA-Z0-9_\0", T(PARAM, .param_prefix=1)),
		LEXER_T("$*", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGV)),
		LEXER_T("$@", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGV2)),
		LEXER_T("$#", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGC)),
		LEXER_T("$?", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_STATUS)),
		LEXER_T("$-", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_OPT)),
		LEXER_T("$!", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_PID)),
		LEXER_T("\\`", T(ESCAPED, '`')),
		LEXER_T("\\$", T(ESCAPED, '$')),
	),

	LEXER_STATE("sh-base-escape", (),
		LEXER_T("\\a", T(ESCAPED, '\a')),
		LEXER_T("\\b", T(ESCAPED, '\b')),
		LEXER_T("\\e", T(ESCAPED, '\x1b')),
		LEXER_T("\\E", T(ESCAPED, '\x1b')),
		LEXER_T("\\f", T(ESCAPED, '\f')),
		LEXER_T("\\n", T(ESCAPED, '\n')),
		LEXER_T("\\r", T(ESCAPED, '\r')),
		LEXER_T("\\t", T(ESCAPED, '\t')),
		LEXER_T("\\v", T(ESCAPED, '\v')),
		LEXER_T("\\\\", T(ESCAPED, '\\')),
		LEXER_T("\\'", T(ESCAPED, '\'')),
		LEXER_T("\\\"", T(ESCAPED, '"')),
		LEXER_T("\\", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_OCTAL))),
		LEXER_T("\\o", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_OCTAL))),
		LEXER_T("\\O", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_OCTAL))),
		LEXER_T("\\x", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_HEXA))),
		LEXER_T("\\X", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_HEXA))),
		LEXER_T("\\c", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_CONTROL))),
		LEXER_T("\\C", T(ESCAPE_SEQUENCE, .escape_sequence=SH(ESCAPE_CONTROL))),
	),

	LEXER_STATE("sh-base-text", ("sh-base-subst"),
		LEXER_T(" ", T(SPACE)),
		LEXER_T("\t", T(SPACE)),
		LEXER_T("\\ ", T(ESCAPED, ' ')),
		LEXER_T("\\\t", T(ESCAPED, '\t')),

		LEXER_T("\"", T(STRING, .string=SH(STRING_NORMAL))),
		LEXER_T("\\\"", T(ESCAPED, '"')),
		LEXER_T("'", T(STRING, .string=SH(STRING_SIMPLE))),
		LEXER_T("\\'", T(ESCAPED, '\'')),
		LEXER_T("$'", T(STRING, .string=SH(STRING_ANSI))),

		LEXER_T("#", T(COMMENT)),
		LEXER_T("\\#", T(ESCAPED, '#')),

		LEXER_T("\\", T(BACKSLASH)),
		LEXER_T("\\\\", T(ESCAPED, '\\')),
	),

	LEXER_STATE("sh-base-cmd", ("sh-base-text"),
		LEXER_T("|", T(PIPELINE_END)),
		LEXER_T("&&", T(LIST_END, .list_end=SH_LIST_AND)),
		LEXER_T("||", T(LIST_END, .list_end=SH_LIST_OR)),
		LEXER_T(";", T(COMPOUND_END, .compound_end=SH(COMPOUND_SEMICOLON))),
		LEXER_T("&", T(COMPOUND_END, .compound_end=SH(COMPOUND_AMPERSAND))),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(COMPOUND_NEWLINE))),

		LEXER_T("(", T(PARENTHESIS_OPEN)),
		LEXER_T(")", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL))),
		LEXER_T("\\(", T(ESCAPED, '(')),
		LEXER_T("\\)", T(ESCAPED, ')')),

		LEXER_T(">", T(REDIR, .redir=SH_REDIR_OUTPUT)),
		LEXER_T(">|", T(REDIR, .redir=SH_REDIR_OUTPUT_CLOBBER)),
		LEXER_T(">&", T(REDIR, .redir=SH_REDIR_OUTPUT_FD)),
		LEXER_T(">>", T(REDIR, .redir=SH_REDIR_APPEND)),
		LEXER_T("<", T(REDIR, .redir=SH_REDIR_INPUT)),
		LEXER_T("<&", T(REDIR, .redir=SH_REDIR_INPUT_FD)),
		LEXER_T("<>", T(REDIR, .redir=SH_REDIR_OPEN)),
		LEXER_T("<<", T(HEREDOC)),

		LEXER_T("\\;", T(ESCAPED, ';')),
		LEXER_T("\\|", T(ESCAPED, '|')),
		LEXER_T("\\&", T(ESCAPED, '&')),
		LEXER_T("\\>", T(ESCAPED, '>')),
		LEXER_T("\\<", T(ESCAPED, '<')),
	),
);

static t_lexer_def		g_sh_compound_lexer = LEXER_DEF(
	(&g_sh_lexer_base), "sh-compound",

	LEXER_STATE("sh-compound", ("sh-base-cmd")),
);

bool			sh_parse(t_in *in, t_sh_compound *dst, t_sh_parse_err *err)
{
	t_lexer_frame			frame;
	t_sh_parser				p;
	bool					r;

	p = (t_sh_parser){LEXER(in), err, false}; // TODO: receive lexer from args
	ft_lexer_push(&p.l, &frame, &g_sh_compound_lexer);
	sh_ignore_newlines(&p);
	r = sh_parse_compound(&p, dst, false)
		&& (p.l.eof || (SH_T_EQU(&p, COMPOUND_END, COMPOUND_NEWLINE)
			|| (sh_destroy_compound(dst), sh_parse_error(&p, SH_E_UNEXPECTED))));
	ft_lexer_pop(&p.l, &frame);
	ft_lexer_destroy(&p.l);
	return (r);
}
