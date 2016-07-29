/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_def.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 18:21:47 by juloo             #+#    #+#             */
/*   Updated: 2016/07/29 01:21:16 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_vector.h"
#include "ft/parser_def.h"

#include "p_sh_parser.h"

static bool		sh_parse_frame_unsupported(t_parse_data *p,
					void const *data, void *param);

#define T(NAME, TYPE, DATA, ...)	PARSER_T(NAME, &((t_sh_token_data){SH_PARSE_T_##TYPE, {DATA}}), ##__VA_ARGS__)

static t_vector const	g_sh_parser = VECTORC(((t_parser_def const[]){

	PARSER_DEF("sh-base-subst", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("${", NONE, 0, .parser="sh-expr"),
			T("$(", NONE, 0, .parser="sh-sub"),
			T("$((", NONE, 0, .parser="sh-math"),
			T("`", NONE, 0, .parser="sh-backquote"),
			T("\\`", ESCAPED, 0),
			T("\\$", ESCAPED, 0),
		),
	),

	PARSER_DEF("sh-base-text", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("\\", BACKSLASH, 0),
			T("\"", NONE, 0, .parser="sh-string"),
			T("'", NONE, 0, .parser="sh-string-single"),
			T("\\\"", ESCAPED, 0),
			T("\\'", ESCAPED, 0),
		),
	),

	PARSER_DEF("sh-cmd-base", NULL, NULL,
		PARSER_INHERIT("sh-base-subst", "sh-base-text"),
		.tokens = PARSER_DEF_T(
			T("&&", END, .end=SH_PARSE_T_END_AND, .end=true),
			T("||", END, .end=SH_PARSE_T_END_OR, .end=true),
			T("|", END, .end=SH_PARSE_T_END_PIPE, .end=true),
			T(";", END, .end=SH_PARSE_T_END_SEMICOLON, .end=true),
			T("&", END, .end=SH_PARSE_T_END_AMPERSAND, .end=true),
			T("\n", END, .end=SH_PARSE_T_END_NEWLINE, .end=true),

			T(" ", SPACE, 0),
			T("\t", SPACE, 0),
			T("#", NONE, 0, .end=true, .parser="sh-comment"),

			T(">", REDIR, .redir=SH_REDIR_OUTPUT),
			T(">|", REDIR, .redir=SH_REDIR_OUTPUT_CLOBBER),
			T(">>", REDIR, .redir=SH_REDIR_APPEND),
			T("<", REDIR, .redir=SH_REDIR_INPUT),
			T("<&", REDIR, .redir=SH_REDIR_INPUT_FD),
			T(">&", REDIR, .redir=SH_REDIR_OUTPUT_FD),
			T("<>", REDIR, .redir=SH_REDIR_OPEN),
			T("<<", HEREDOC, 0),

			T("\\ ", ESCAPED, 0),
			T("\\\t", ESCAPED, 0),
			T("\\\n", ESCAPED, 0),
			T("\\;", ESCAPED, 0),
			T("\\|", ESCAPED, 0),
			T("\\&", ESCAPED, 0),
			T("\\>", ESCAPED, 0),
			T("\\<", ESCAPED, 0),

		),
	),

	PARSER_DEF("sh-compound", NULL, V(&sh_parse_frame_compound),
		PARSER_INHERIT("sh-cmd-base"),
	),

	PARSER_DEF("sh-sub", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-cmd-base"),
		.tokens = PARSER_DEF_T(
			T(")", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-sub-quoted", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-sub"),
	),

	PARSER_DEF("sh-backquote", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-cmd-base"),
		.tokens = PARSER_DEF_T(
			T("`", NONE, 0, .end=true),
			T("\\`", NONE, 0, .parser="sh-backquote-rev"),
		),
	),

	PARSER_DEF("sh-backquote-quoted", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-backquote"),
	),

	PARSER_DEF("sh-backquote-rev", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-cmd-base"),
		.tokens = PARSER_DEF_T(
			T("\\`", NONE, 0, .end=true),
			T("`", NONE, 0, .parser="sh-backquote"),
		),
	),

	PARSER_DEF("sh-expr", NULL, V(&sh_parse_frame_unsupported),
		.tokens = PARSER_DEF_T(
			T("}", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-math", NULL, V(&sh_parse_frame_unsupported),
		.tokens = PARSER_DEF_T(
			T("))", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-string", NULL, V(&sh_parse_frame_unsupported),
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("$(", NONE, 0, .parser="sh-sub-quoted"),
			T("`", NONE, 0, .parser="sh-backquote-quoted"),
			T("\"", NONE, 0, .end=true),
			T("\\\"", ESCAPED, 0),
		),
	),

	PARSER_DEF("sh-string-single", NULL, V(&sh_parse_frame_unsupported),
		.tokens = PARSER_DEF_T(
			T("'", NONE, 0, .end=true),
		),
	),

	PARSER_DEF("sh-comment", NULL, V(&sh_parse_frame_unsupported),
		.tokens = PARSER_DEF_T(
			T("\n", NONE, 0, .end=true),
		),
	),

}));

static bool		sh_parse_frame_unsupported(t_parse_data *p,
					void const *data, void *param)
{
	ASSERT(!"Unsupported parser");
	return (ft_parse_error(p));
	(void)data;
	(void)param;
}

t_parser const	*sh_load_parser(t_sub key)
{
	static t_hmap	*map = NULL;
	t_parser const	*tmp;

	if (map == NULL)
	{
		map = ft_hmapnew(16, &ft_djb2);
		if (!build_parser(map, &g_sh_parser))
			HARD_ASSERT(!"Fail building sh parser");
	}
	if ((tmp = ft_hmapget(map, key).value) == NULL)
		HARD_ASSERT(!"Invalid parser");
	return (tmp);
}
