/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_def.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:32:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/15 14:24:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "ft/ft_vector.h"
#include "ft/parser_def.h"

#include "internal.h"

#define T(STR,T,D,...)	PARSER_T(STR, &C(t_sh_parser_data, SH_PARSE_T_##T, (D)), ##__VA_ARGS__)

static t_vector const	g_sh_parser = VECTORC(((t_parser_def const[]){

	PARSER_DEF("sh-base-subst", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("${", NONE, 0, .parser="sh-expr"),
			T("$(", NONE, 0, .parser="sh-sub"),
			T("$((", NONE, 0, .parser="sh-math"),
			T("`", NONE, 0, .parser="sh-backquote"),
		),
		.match = PARSER_DEF_T(
			T("$?[a-zA-Z_]?*w", PARAM, 0),
			T("$?.", PARAM_SPECIAL, 0),
		),
	),

	PARSER_DEF("sh-cmd", NULL, &sh_parse_frame_cmd,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("&&", NEXT, SH_NEXT_AND, .end=true),
			T("||", NEXT, SH_NEXT_OR, .end=true),
			T("|", NEXT, SH_NEXT_PIPE, .end=true),
			T(";", NEXT, SH_NEXT_NEW, .end=true),
			T("&", AMPERSAND, 0, .end=true),
			T("\n", NEXT, SH_NEXT_NEW, .end=true),
			T(" ", SPACE, 0),
			T("\t", SPACE, 0),
			T("\"", NONE, 0, .parser="sh-string"),
			T("'", NONE, 0, .parser="sh-string-single"),
			T("#", NONE, 0, .end=true, .parser="sh-comment"),

			T(">", REDIR, SH_REDIR_OUTPUT),
			T(">|", REDIR, SH_REDIR_OUTPUT_CLOBBER),
			T(">>", REDIR, SH_REDIR_APPEND),
			T("<", REDIR, SH_REDIR_INPUT),
			T("<<", REDIR, SH_REDIR_HEREDOC),
			T("<&", REDIR, SH_REDIR_DUP_INPUT),
			T(">&", REDIR, SH_REDIR_DUP_OUTPUT),
			T("<>", REDIR, SH_REDIR_OPEN),

			T("\\", BACKSLASH, 0),
			T("\\ ", ESCAPED, 0),
			T("\\\t", ESCAPED, 0),
			T("\\\n", ESCAPED, 0),
			T("\\;", ESCAPED, 0),
			T("\\|", ESCAPED, 0),
			T("\\&", ESCAPED, 0),
			T("\\`", ESCAPED, 0),
			T("\\\"", ESCAPED, 0),
			T("\\'", ESCAPED, 0),

		),
		.match = PARSER_DEF_T(
			T("\\?+w", ESCAPED, 0),
		),
	),

	PARSER_DEF("sh-sub", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T(")", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-backquote", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("`", NONE, 0, .end=true),
			T("\\`", NONE, 0, .parser="sh-backquote-rev"),
		),
	),

	PARSER_DEF("sh-backquote-rev", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("\\`", NONE, 0, .end=true),
			T("`", NONE, 0, .parser="sh-backquote"),
		),
	),

	PARSER_DEF("sh-expr", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("}", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-math", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("))", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-string", NULL, &sh_parse_frame_string,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("\"", NONE, 0, .end=true),
			T("\\\"", ESCAPED, 0),
		),
	),

	PARSER_DEF("sh-string-single", NULL, &sh_parse_frame_string,
		.tokens = PARSER_DEF_T(
			T("'", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-comment", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("\n", NONE, 0, .end=true),
		),
	),

}));

t_parser const	*load_sh_parser(void)
{
	static t_parser const	*sh_parser = NULL;
	static t_hmap			*map = NULL;

	if (sh_parser == NULL)
	{
		if (map == NULL)
			map = ft_hmapnew(10, &ft_djb2);
		if (!build_parser(map, &g_sh_parser))
			return (NULL);
		sh_parser = ft_hmapget(map, SUBC("sh-cmd")).value;
	}
	return (sh_parser);
}
