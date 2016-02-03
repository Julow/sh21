/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_syntax_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:44:48 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 20:10:20 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "ft/parser_def.h"

#include "syntax_color_loader.h"

t_parser_def const		g_parser_def_utils[] = {
	PARSER_DEF("string", "string",
		.tokens = PARSER_DEF_T(
			PARSER_T("\"", "end", .end=true),
			PARSER_T("\\\"", "escaped.quote"),
		),
	),

	PARSER_DEF("allow-blanks", "blanks",
		.tokens = PARSER_DEF_T(
			PARSER_T(" ", "blank.space"),
			PARSER_T("\t", "blank.tab"),
			PARSER_T("\n", "blank.newline"),
		),
	),
};

t_parser_def const		g_parser_def_sh[] = {
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
};

t_parser_def const		g_parser_def_xml[] = {
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
};

#define DEF(NAME, DEF, ...)	{SUBC(NAME), (DEF), VECTORC(((char const*[]){__VA_ARGS__}))}

static struct // s_syntax_color_def
{
	t_sub				name;
	t_vector			def;
	t_vector			require;
} const			g_parser_defs[] = {
// TODO: static t_syntax_color_def const *g_parser_defs[]
// TODO: 1 syntax per file
	DEF("string", VECTORC(g_parser_def_utils)),
	DEF("allow-blanks", VECTORC(g_parser_def_utils)),
	DEF("sh", VECTORC(g_parser_def_sh), "string", "allow-blanks"),
	DEF("xml", VECTORC(g_parser_def_xml), "string", "allow-blanks"),
};

t_parser const	*load_syntax_color(t_sub name)
{
	static t_hmap	*parser_map = NULL;
	t_parser const	*tmp;
	uint32_t		i;
	uint32_t		j;

	if (parser_map == NULL)
		parser_map = ft_hmapnew(20, &ft_djb2);
	else if ((tmp = ft_hmapget(parser_map, name).value) != NULL)
		return (tmp);

	i = 0;
	while (i < ARRAY_LEN(g_parser_defs))
	{
		if (SUB_EQU(g_parser_defs[i].name, name))
		{
			j = 0;
			while (j < g_parser_defs[i].require.length)
				if (load_syntax_color(*(t_sub const*)VECTOR_GET(g_parser_defs[i].require, j++)) == NULL)
					return (NULL);
			if (!build_parser(parser_map, &g_parser_defs[i].def))
				return (NULL);
			break ;
		}
		i++;
	}
	return (ft_hmapget(parser_map, name).value);
}
