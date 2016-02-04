/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:18:24 by juloo             #+#    #+#             */
/*   Updated: 2016/02/04 00:35:51 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_syntax_color_def const	g_syntax_color_sh = SYNTAX_COLOR("sh", "sh",
	SYNTAX_COLOR_DEPEND(SUBC("utils")),
	.def = VECTORC(((t_parser_def[]){

		PARSER_DEF("sh", "sh",
			.inherit = SUBC("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\\\"", "escaped.quote"),
				PARSER_T("\\#", "escaped.comment"),
				PARSER_T("\\\'", "escaped.quote.simple"),
				PARSER_T("\\$", "escaped.dollar"),
				PARSER_T("(", "start", .parser="sh-sub"),
				PARSER_T("$(", "start", .parser="sh-sub"),
				PARSER_T("`", "start", .parser="sh-backquote"),
				PARSER_T("$((", "start", .parser="sh-math"),
				PARSER_T("${", "start", .parser="sh-expr"),
				PARSER_T(";", "op.semicolon"),
				PARSER_T("\"", "start", .parser="sh-string"),
				PARSER_T("'", "start", .parser="sh-string-simple"),
				PARSER_T("#", "start", .parser="sh-comment"),
				PARSER_T("&&", "op.and"),
				PARSER_T("&", "op.async"),
				PARSER_T("|", "op.pipe"),
				PARSER_T("||", "op.or"),
				PARSER_T("<", "redir.left"),
				PARSER_T("<<", "redir.heredoc"),
				PARSER_T(">", "redir.right"),
				PARSER_T(">>", "redir.right.double"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("$?[a-zA-Z_]?*w", "var"),
				PARSER_T("$?.", "var"),
			),
		),

		PARSER_DEF("sh-sub", "sub",
			.inherit = SUBC("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "end", .end=true),
			),
		),

		PARSER_DEF("sh-backquote", "backquote",
			.inherit = SUBC("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T("`", "end", .end=true),
			),
		),

		PARSER_DEF("sh-string", "string",
			.inherit = SUBC("string"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\\n", "escaped.char"),
				PARSER_T("\\e", "escaped.char"),
				PARSER_T("\\t", "escaped.char"),
				PARSER_T("`", "start", .parser="sh-backquote"),
				PARSER_T("$(", "start", .parser="sh-sub"),
				PARSER_T("$((", "start", .parser="sh-math"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("$?[a-zA-Z_]?*w", "var"),
			),
		),

		PARSER_DEF("sh-string-simple", "string.simple",
			.tokens = PARSER_DEF_T(PARSER_T("'", "end", .end=true)),
		),

		PARSER_DEF("sh-comment", "comment",
			.tokens = PARSER_DEF_T(PARSER_T("\n", "endl", .end=true)),
		),

		PARSER_DEF("sh-expr", "expr",
			.tokens = PARSER_DEF_T(
				PARSER_T("}", "end", .end=true),
				PARSER_T("%", "op"),
				PARSER_T("%%", "op"),
				PARSER_T("#", "op"),
			),
		),

		PARSER_DEF("sh-math", "math",
			.inherit = SUBC("math"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "error"),
				PARSER_T("))", "end", .end=true),
			),
		),

		PARSER_DEF("math", "math",
			.tokens = PARSER_DEF_T(
				PARSER_T("(", "brace", .parser="math-brace"),
				PARSER_T("+", "op.plus"),
				PARSER_T("-", "op.minus"),
				PARSER_T("*", "op.mult"),
				PARSER_T("/", "op.div"),
				PARSER_T("%", "op.mod"),
				PARSER_T(" ", "space"),
				PARSER_T("\t", "space"),
				PARSER_T("\n", "space"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("$?[a-zA-Z_]?*w", "var"),
				PARSER_T("?b?+d?\?(.?*d?\?'f')?b", "number"),
			),
		),

		PARSER_DEF("math-brace", "math",
			.inherit = SUBC("math"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "close", .end=true),
			),
		),

	})),
);
