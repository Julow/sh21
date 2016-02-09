/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xml.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:18:24 by juloo             #+#    #+#             */
/*   Updated: 2016/02/09 12:45:46 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_syntax_color_def const	g_syntax_color_xml = SYNTAX_COLOR("xml", "xml",
	SYNTAX_COLOR_DEPEND(SUBC("utils")),
	.def = VECTORC(((t_parser_def[]){

/*
** ========================================================================== **
** Base
*/

		PARSER_DEF("xml", "xml", NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T("<", "start", .parser="xml-markup"),
				PARSER_T(">", "error"),
				PARSER_T("</", "error"),
				PARSER_T("-->", "error"),
				PARSER_T("<!--", "start", .parser="xml-comment"),
				PARSER_T("<?", "start", .parser="xml-header"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error"),
			),
		),

		PARSER_DEF("xml-comment", "comment", NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T("-->", "end", .end=true),
			),
		),

/*
** ========================================================================== **
** Markup
*/

		PARSER_DEF("xml-markup-base", "markup", NULL,
			PARSER_INHERIT("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("/>", "end", .end=true),
				PARSER_T(">", "start", .end=true, .parser="xml-group"), // TODO: tail
			),
		),

		PARSER_DEF("xml-markup", "markup", NULL,
			PARSER_INHERIT("xml-markup-base"),
			.match = PARSER_DEF_T(
				PARSER_T("?!+w", "error"),
				PARSER_T("?b?+w", "identifier", .end=true, .parser="xml-markup-params"),
			),
		),

		PARSER_DEF("xml-markup-params", "params", NULL,
			PARSER_INHERIT("xml-markup-base"),
			.match = PARSER_DEF_T(
				PARSER_T("?b?+w", "identifier.key", .parser="xml-markup-param-key"),
				PARSER_T("?+.", "error", .end=true),
			),
		),

/*
** ========================================================================== **
** Params
*/

		PARSER_DEF("xml-markup-param-key", "param-key", NULL,
			PARSER_INHERIT("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("=", "op.equal", .end=true, .parser="xml-markup-param-value"), // TODO: tail
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error", .end=true),
			),
		),

		PARSER_DEF("xml-markup-param-value", "param-value", NULL,
			PARSER_INHERIT("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("\"", "start", .end=true, .parser="string"),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?+.", "error", .end=true),
			),
		),

/*
** ========================================================================== **
** Group
*/

		PARSER_DEF("xml-group", "group", NULL,
			PARSER_INHERIT("xml"),
			.tokens = PARSER_DEF_T(
				PARSER_T("</", "start", .end=true, .parser="xml-group-end"),
			),
		),

		PARSER_DEF("xml-group-end", "group-end", NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T(">", "end", .end=true),
			),
		),

/*
** ========================================================================== **
** Header
*/

		PARSER_DEF("xml-header", "header", NULL,
			PARSER_INHERIT("allow-blanks"),
			.tokens = PARSER_DEF_T(
				PARSER_T("?>", "end", .end=true),
			),
			.match = PARSER_DEF_T(
				PARSER_T("?b?+w", "identifier", .end=true, .parser="xml-header-params"),
			),
		),

		PARSER_DEF("xml-header-params", "header", NULL,
			PARSER_INHERIT("xml-header"),
			.match = PARSER_DEF_T(
				PARSER_T("?b?+w", "identifier.key", .parser="xml-markup-param-key"),
			),
		),

	})),
);
