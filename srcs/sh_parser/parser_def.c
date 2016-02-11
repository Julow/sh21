/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_def.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:32:08 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 19:27:35 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "ft/ft_vector.h"
#include "ft/parser_def.h"

#include "internal.h"

#define T(STR,T,...)	PARSER_T(STR, V(SH_T_##T), ##__VA_ARGS__)

static t_vector const	g_sh_parser = VECTORC(((t_parser_def const[]){

	PARSER_DEF("sh-base-subst", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("${", BEGIN, .parser="sh-expr"),
			T("$(", BEGIN, .parser="sh-sub"),
			T("$((", BEGIN, .parser="sh-math"),
			T("`", BEGIN, .parser="sh-backquote"),
		),
		.match = PARSER_DEF_T(
			T("$?[a-zA-Z_]?*w", SUBST_PARAM),
			T("$?.", SUBST_PARAM_SPECIAL),
		),
	),

	PARSER_DEF("sh-cmd", V(SH_P_SHELL), &sh_parse_cmd,
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
			T("\"", BEGIN, .parser="sh-string"),
			T("'", BEGIN, .parser="sh-string-single"),
			T("#", BEGIN, .end=true, .parser="sh-comment"),

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
	),

	PARSER_DEF("sh-sub", V(SH_P_SUBSHELL), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T(")", END, .end=true),
		),
	),

	PARSER_DEF("sh-backquote", V(SH_P_BACKQUOTE), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("`", END, .end=true),
			T("\\`", BEGIN, .parser="sh-backquote-rev"),
		),
	),

	PARSER_DEF("sh-backquote-rev", V(SH_P_BACKQUOTE), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("\\`", END, .end=true),
			T("`", BEGIN, .parser="sh-backquote"),
		),
	),

	PARSER_DEF("sh-expr", V(SH_P_EXPR), &sh_parse_ignore,
		.tokens = PARSER_DEF_T(
			T("}", END, .end=true),
		),
	),

	PARSER_DEF("sh-math", V(SH_P_MATH), &sh_parse_ignore,
		.tokens = PARSER_DEF_T(
			T("))", END, .end=true),
		),
	),

	PARSER_DEF("sh-string", V(SH_P_STRING), &sh_parse_string,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("\"", END, .end=true),
			T("\\\"", ESCAPED),
		),
	),

	PARSER_DEF("sh-string-single", V(SH_P_STRING_SINGLE), &sh_parse_string,
		.tokens = PARSER_DEF_T(
			T("'", END, .end=true),
		),
	),

	PARSER_DEF("sh-comment", V(SH_P_IGNORE), &sh_parse_ignore,
		.tokens = PARSER_DEF_T(
			T("\n", END, .end=true),
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
