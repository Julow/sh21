/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/30 23:26:24 by juloo             #+#    #+#             */
/*   Updated: 2016/09/07 16:18:59 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/lexer.h"
#include "p_sh_parser.h"

#include <stdlib.h>

#define T(T, ...)	(&SH_PARSE_T(T, ##__VA_ARGS__))

static t_lexer_def const	g_sh_lexer = LEXER_DEF(

	LEXER_STATE("sh-base", (),
		LEXER_T("", T(TEXT)),
	),

	LEXER_STATE("sh-base-subst", ("sh-base"),
		LEXER_T("${", T(SUBST, .subst=SH(SUBST_PARAM)), .push="sh-subst-param"),
		LEXER_T("$(", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-subshell"),
		LEXER_T("$((", T(SUBST, .subst=SH(SUBST_MATH))),
		LEXER_T("`", T(SUBST, .subst=SH(SUBST_SUBSHELL)), .push="sh-subst-backquote"),
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

		LEXER_T("\"", T(STRING, .string=SH(STRING_DOUBLE)), .push="sh-string"),
		LEXER_T("\\\"", T(ESCAPED, '"')),
		LEXER_T("'", T(STRING, .string=SH(STRING_SIMPLE)), .push="sh-string-single"),
		LEXER_T("\\'", T(ESCAPED, '\'')),
		LEXER_T("$'", T(STRING, .string=SH(STRING_SIMPLE)), .push="sh-string-ansi"),

		LEXER_T("#", T(COMMENT), .push="sh-comment"),
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

	LEXER_STATE("sh-subst-param", (),
		LEXER_T("\0000-9\0", T(PARAM_POS, .param_prefix=0), .push="sh-subst-param-op", .pop=true),
		LEXER_T("\0a-zA-Z_\0\0+a-zA-Z0-9_\0", T(PARAM, .param_prefix=0), .push="sh-subst-param-op", .pop=true),
		LEXER_T("*", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGV), .push="sh-subst-param-op", .pop=true),
		LEXER_T("@", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGV2), .push="sh-subst-param-op", .pop=true),
		LEXER_T("#", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_ARGC), .push="sh-subst-param-op", .pop=true),
		LEXER_T("#\0a-zA-Z_\0\0+a-zA-Z0-9_\0", T(PARAM_LENGTH, .param_prefix=1), .push="sh-subst-param-end", .pop=true),
		LEXER_T("?", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_STATUS), .push="sh-subst-param-op", .pop=true),
		LEXER_T("-", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_OPT), .push="sh-subst-param-op", .pop=true),
		LEXER_T("!", T(PARAM_SPECIAL, SH_SPECIAL_PARAM_PID), .push="sh-subst-param-op", .pop=true),
	),

	LEXER_STATE("sh-subst-param-end", (),
		LEXER_T("}", T(SUBST_PARAM_END), .pop=true),
	),

	LEXER_STATE("sh-subst-param-op", ("sh-subst-param-end"),
		LEXER_T(":-", T(SUBST_PARAM_OP, SH_SUBST_PARAM_DEF_NULL), .push="sh-subst-param-str", .pop=true),
		LEXER_T("-", T(SUBST_PARAM_OP, SH_SUBST_PARAM_DEF_UNSET), .push="sh-subst-param-str", .pop=true),
		LEXER_T(":=", T(SUBST_PARAM_OP, SH_SUBST_PARAM_ASSIGN_NULL), .push="sh-subst-param-str", .pop=true),
		LEXER_T("=", T(SUBST_PARAM_OP, SH_SUBST_PARAM_ASSIGN_UNSET), .push="sh-subst-param-str", .pop=true),
		LEXER_T(":+", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REPL_NULL), .push="sh-subst-param-str", .pop=true),
		LEXER_T("+", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REPL_UNSET), .push="sh-subst-param-str", .pop=true),
		LEXER_T(":?", T(SUBST_PARAM_OP, SH_SUBST_PARAM_ERR_NULL), .push="sh-subst-param-str", .pop=true),
		LEXER_T("?", T(SUBST_PARAM_OP, SH_SUBST_PARAM_ERR_UNSET), .push="sh-subst-param-str", .pop=true),
		LEXER_T("^", T(SUBST_PARAM_OP, SH_SUBST_PARAM_UPPER_FIRST), .push="sh-subst-param-end", .pop=true),
		LEXER_T("^^", T(SUBST_PARAM_OP, SH_SUBST_PARAM_UPPER), .push="sh-subst-param-end", .pop=true),
		LEXER_T(",", T(SUBST_PARAM_OP, SH_SUBST_PARAM_LOWER_FIRST), .push="sh-subst-param-end", .pop=true),
		LEXER_T(",,", T(SUBST_PARAM_OP, SH_SUBST_PARAM_LOWER), .push="sh-subst-param-end", .pop=true),
		LEXER_T("~", T(SUBST_PARAM_OP, SH_SUBST_PARAM_INVCASE_FIRST), .push="sh-subst-param-end", .pop=true),
		LEXER_T("~~", T(SUBST_PARAM_OP, SH_SUBST_PARAM_INVCASE), .push="sh-subst-param-end", .pop=true),
		LEXER_T("#", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REM_BEGIN), .push="sh-subst-param-str", .pop=true),
		LEXER_T("##", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REM_BEGIN_LONG), .push="sh-subst-param-str", .pop=true),
		LEXER_T("%", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REM_END), .push="sh-subst-param-str", .pop=true),
		LEXER_T("%%", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REM_END_LONG), .push="sh-subst-param-str", .pop=true),
		LEXER_T("/", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REPL_FIRST), .push="sh-subst-param-replace", .pop=true),
		LEXER_T("/%", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REPL_LAST), .push="sh-subst-param-replace", .pop=true),
		LEXER_T("//", T(SUBST_PARAM_OP, SH_SUBST_PARAM_REPL_ALL), .push="sh-subst-param-replace", .pop=true),
	),

	LEXER_STATE("sh-subst-param-str", ("sh-base-text", "sh-subst-param-end"),
		LEXER_T("\\}", T(ESCAPED, '}')),
	),

	LEXER_STATE("sh-subst-param-replace", ("sh-subst-param-str"),
		LEXER_T("/", T(SUBST_PARAM_OP, 0)),
		LEXER_T("\\/", T(ESCAPED, '/')),
	),

	LEXER_STATE("sh-string", ("sh-base-subst"),
		LEXER_T("\"", T(STRING, .string=SH(STRING_END)), .pop=true),
		LEXER_T("\\\"", T(ESCAPED, '"')),
	),

	LEXER_STATE("sh-string-single", ("sh-base"),
		LEXER_T("'", T(STRING, .string=SH(STRING_END)), .pop=true),
	),

	LEXER_STATE("sh-string-ansi", ("sh-string-single", "sh-base-escape")),

	LEXER_STATE("sh-comment", ("sh-base"),
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
		&& (p.l.eof || (SH_T_EQU(&p, COMPOUND_END, COMPOUND_NEWLINE)
			|| (sh_destroy_compound(dst), sh_parse_error(&p, SH_E_UNEXPECTED))));
	ft_lexer_pop(&p.l, NULL);
	ft_lexer_destroy(&p.l);
	return (r);
}
