/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:18:24 by juloo             #+#    #+#             */
/*   Updated: 2016/02/04 00:35:39 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_syntax_color_def const	g_syntax_color_xml = SYNTAX_COLOR("xml", "xml",
	SYNTAX_COLOR_DEPEND(SUBC("utils")),
	.def = VECTORC(((t_parser_def[]){

		PARSER_DEF("xml", "xml",
			.tokens = PARSER_DEF_T(
				PARSER_T("<", "start", .parser="xml-markup"),
				PARSER_T(">", "error"),
				PARSER_T("</", "error"),
				PARSER_T("-->", "error"),
				PARSER_T("<!--", "start", .parser="xml-comment"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error"),
			),
		),

		PARSER_DEF("xml-markup-base", "markup",
			.inherit = SUBC("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("/>", "end", .end=true),
				PARSER_T(">", "start", .end=true, .parser="xml-group"),
			),
		),

		PARSER_DEF("xml-markup", "markup",
			.inherit = SUBC("xml-markup-base"),
			.match = PARSER_DEF_T(
				PARSER_T("?!+w", "error"),
				PARSER_T("?b?+w?b", "identifier", .end=true, .parser="xml-markup-params"),
			),
		),

		PARSER_DEF("xml-markup-params", "params",
			.inherit = SUBC("xml-markup-base"),
			.match = PARSER_DEF_T(
				PARSER_T("?b?+w?b", "identifier.key", .parser="xml-markup-params-key"),
				PARSER_T("?+.", "error", .end=true),
			),
		),

		PARSER_DEF("xml-markup-params-key", "params",
			.inherit = SUBC("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("=", "op.equal", .end=true, .parser="xml-markup-params-value"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error", .end=true),
			),
		),

		PARSER_DEF("xml-markup-params-value", "params",
			.inherit = SUBC("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\"", "start", .end=true, .parser="string"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error", .end=true),
			),
		),

		PARSER_DEF("xml-group", "group",
			.inherit = SUBC("xml"),
			.tokens = PARSER_DEF_T(
				PARSER_T("</", "end.close", .end=true, .parser="xml-group-end"),
			),
		),

		PARSER_DEF("xml-group-end", "group-end",
			.tokens = PARSER_DEF_T(
				PARSER_T(">", "end", .end=true),
			),
		),

		PARSER_DEF("xml-comment", "comment",
			.tokens = PARSER_DEF_T(
				PARSER_T("-->", "end", .end=true),
			),
		),

	})),
);
