/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_def.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:32:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 11:58:11 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "ft/ft_vector.h"
#include "ft/parser_def.h"

#include "internal.h"

#define T(STR,T,...)	PARSER_T(STR, V(SH_PARSE_T_##T), ##__VA_ARGS__)

static t_vector const	g_sh_parser = VECTORC(((t_parser_def const[]){

	PARSER_DEF("sh-base-subst", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("${", NONE, .parser="sh-expr"),
			T("$(", NONE, .parser="sh-sub"),
			T("$((", NONE, .parser="sh-math"),
			T("`", NONE, .parser="sh-backquote"),
		),
		.match = PARSER_DEF_T(
			T("$?[a-zA-Z_]?*w", PARAM),
			T("$?.", PARAM_SPECIAL),
		),
	),

	PARSER_DEF("sh-cmd", NULL, &sh_parse_frame_cmd,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("&&", AND, .end=true),
			T("||", OR, .end=true),
			T("|", PIPE, .end=true),
			T(";", SEMICOLON, .end=true),
			T("&", AMPERSAND, .end=true),
			T("\n", NEWLINE, .end=true),
			T(" ", SPACE),
			T("\t", SPACE),
			T("\"", NONE, .parser="sh-string"),
			T("'", NONE, .parser="sh-string-single"),
			T("#", NONE, .end=true, .parser="sh-comment"),

			T("\\", BACKSLASH),
			T("\\ ", ESCAPED),
			T("\\\t", ESCAPED),
			T("\\\n", ESCAPED),
			T("\\;", ESCAPED),
			T("\\|", ESCAPED),
			T("\\&", ESCAPED),
			T("\\`", ESCAPED),
			T("\\\"", ESCAPED),
			T("\\'", ESCAPED),

		),
		.match = PARSER_DEF_T(
			T("\\?+w", ESCAPED),
		),
	),

	PARSER_DEF("sh-sub", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T(")", NONE, .end=true),
		),
	),

	PARSER_DEF("sh-backquote", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("`", NONE, .end=true),
			T("\\`", NONE, .parser="sh-backquote-rev"),
		),
	),

	PARSER_DEF("sh-backquote-rev", NULL, &sh_parse_frame_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("\\`", NONE, .end=true),
			T("`", NONE, .parser="sh-backquote"),
		),
	),

	PARSER_DEF("sh-expr", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("}", NONE, .end=true),
		),
	),

	PARSER_DEF("sh-math", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("))", NONE, .end=true),
		),
	),

	PARSER_DEF("sh-string", NULL, &sh_parse_frame_string,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("\"", NONE, .end=true),
			T("\\\"", ESCAPED),
		),
	),

	PARSER_DEF("sh-string-single", NULL, &sh_parse_frame_string,
		.tokens = PARSER_DEF_T(
			T("'", NONE, .end=true),
		),
	),

	PARSER_DEF("sh-comment", NULL, &sh_parse_frame_ignore,
		.tokens = PARSER_DEF_T(
			T("\n", NONE, .end=true),
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
