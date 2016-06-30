/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:18:24 by juloo             #+#    #+#             */
/*   Updated: 2016/06/30 19:09:45 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_syntax_color_def const	g_syntax_color_sh = SYNTAX_COLOR("sh", "sh",
	SYNTAX_COLOR_DEPEND(SUBC("utils")),
	.def = VECTORC(((t_parser_def[]){

		PARSER_DEF("sh-allow-subst", NULL, NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T("$(", "start", .parser="sh-sub"),
				PARSER_T("`", "start", .parser="sh-backquote"),
				PARSER_T("$((", "start", .parser="sh-math"),
				PARSER_T("${", "start", .parser="sh-subst"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("$?[a-zA-Z_]?*w", "subst.param"),
				PARSER_T("$?.", "subst.param"),
			),
		),

		PARSER_DEF("sh-base", "sh", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-allow-subst"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\\\"", "escaped.quote"),
				PARSER_T("\\#", "escaped.comment"),
				PARSER_T("\\\'", "escaped.quote.simple"),
				PARSER_T("\\$", "escaped.dollar"),
				PARSER_T("(", "start", .parser="sh-sub"),
				PARSER_T("\"", "start", .parser="sh-string"),
				PARSER_T("'", "start", .parser="sh-string-simple"),
				PARSER_T("#", "start", .parser="sh-comment"),
				PARSER_T(";", "op.semicolon", .end=true, .parser="sh"),
				PARSER_T("&&", "op.and", .end=true, .parser="sh"),
				PARSER_T("&", "op.async", .end=true, .parser="sh"),
				PARSER_T("|", "op.pipe", .end=true, .parser="sh"),
				PARSER_T("||", "op.or", .end=true, .parser="sh"),
				PARSER_T("<", "redir.left"),
				PARSER_T("<<", "redir.heredoc"),
				PARSER_T(">", "redir.right"),
				PARSER_T(">>", "redir.right.double"),
				PARSER_T("\n", "newline", .end=true, .parser="sh"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+s", "space"),
			),
		),

		PARSER_DEF("sh", "sh", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-base"),
			.match = PARSER_DEF_T(
				PARSER_T("?^?*swhile?b", "keyword.while"),
				PARSER_T("?^?*sfor?b", "keyword.for", .parser="sh-control-for"),
				PARSER_T("?^?*sif?b", "keyword.if"),
				PARSER_T("?^?*suntil?b", "keyword.until"),
				PARSER_T("?^?*s?!+s", "identifier"),
			),
		),

		PARSER_DEF("sh-sub", "sub", &syntax_color_parser_begin,
			PARSER_INHERIT("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "end", .end=true),
			),
		),

		PARSER_DEF("sh-backquote", "backquote", &syntax_color_parser_begin,
			PARSER_INHERIT("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\\`", "begin", .parser="sh-backquote-rev"),
				PARSER_T("`", "end", .end=true),
			),
		),

		PARSER_DEF("sh-backquote-rev", "backquote", &syntax_color_parser_begin,
			PARSER_INHERIT("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T("`", "begin", .parser="sh-backquote"),
				PARSER_T("\\`", "end", .end=true),
			),
		),

		PARSER_DEF("sh-string", "string", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-allow-subst"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\"", "end", .end=true),
				PARSER_T("\\\"", "escaped.quote"),
				PARSER_T("\\n", "escaped.char"),
				PARSER_T("\\e", "escaped.char"),
				PARSER_T("\\t", "escaped.char"),
			),
		),

		PARSER_DEF("sh-string-simple", "string.simple", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(PARSER_T("'", "end", .end=true)),
		),

		PARSER_DEF("sh-comment", "comment", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(PARSER_T("\n", "endl", .end=true)),
		),

		PARSER_DEF("sh-subst", "subst", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(
				PARSER_T("}", "end", .end=true),
				PARSER_T("%", "op"),
				PARSER_T("%%", "op"),
				PARSER_T("#", "op"),
			),
		),

		PARSER_DEF("sh-math-base", NULL, NULL,
			PARSER_INHERIT("sh-allow-subst"),
			.tokens = PARSER_DEF_T(
				PARSER_T("(", "begin", .parser="sh-math-sub"),
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
				PARSER_T("?b?+d?\?(.?*d?\?'f')?b", "number"),
			),
		),

		PARSER_DEF("sh-math", "math", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-math-base"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "error"),
				PARSER_T("))", "end", .end=true),
			),
		),

		PARSER_DEF("sh-math-sub", "math", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-math-base"),
			.tokens = PARSER_DEF_T(
				PARSER_T(")", "end", .end=true),
			),
		),

		PARSER_DEF("sh-control-for", "control.for", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(
				PARSER_T(" ", "space"),
				PARSER_T("\t", "space"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?b?+w?b", "identifier", .end=true, .parser="sh-control-for-in"),
				PARSER_T("?+.", "error"),
			),
		),

		PARSER_DEF("sh-control-for-in", "control.for", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(
				PARSER_T(" ", "space"),
				PARSER_T("\t", "space"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?bin?b", "keyword.in", .end=true, .parser="sh-control-for-list"),
			),
		),

		PARSER_DEF("sh-control-for-list", "control.for", &syntax_color_parser_begin,
			PARSER_INHERIT("sh-base"),
			.tokens = PARSER_DEF_T(
				PARSER_T(";", "op.semicolon", .end=true, .parser="sh-control-do"),
				PARSER_T("&&", "error"),
				PARSER_T("&", "error"),
				PARSER_T("|", "error"),
				PARSER_T("||", "error"),
				PARSER_T("\n", "newline", .end=true, .parser="sh-control-do"),
			),
		),

		PARSER_DEF("sh-control-do", "control.do", &syntax_color_parser_begin,
			.tokens = PARSER_DEF_T(
				PARSER_T(" ", "space"),
				PARSER_T("\t", "space"),
				PARSER_T("\n", "newline"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?bdo?b", "keyword.do", .end=true, .parser="sh-control-done"),
			),
		),

		PARSER_DEF("sh-control-done", "control.for", &syntax_color_parser_begin,
			PARSER_INHERIT("sh"),
			.tokens = PARSER_DEF_T(
				PARSER_T(";", "op.semicolon", .end=true, .parser="sh-control-done"),
				PARSER_T("&&", "error"),
				PARSER_T("&", "error"),
				PARSER_T("|", "error"),
				PARSER_T("||", "error"),
				PARSER_T("\n", "newline", .end=true, .parser="sh-control-done"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?^?*sdone?b", "keyword.done"),
			),
		),

	})),
);
