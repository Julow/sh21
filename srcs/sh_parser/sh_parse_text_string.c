/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_string.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 18:01:46 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/12 22:36:25 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static t_lexer_def const	g_sh_text_string_lexer_base = LEXER_DEF(
	(&g_sh_lexer_base), NULL,

	LEXER_STATE("sh-string", ("sh-base-subst"),
		LEXER_T("\"", T(STRING, .string=SH(STRING_END))),
		LEXER_T("\\\"", T(ESCAPED, '"')),
	),

	LEXER_STATE("sh-string-single", ("sh-base"),
		LEXER_T("'", T(STRING, .string=SH(STRING_END))),
	),

	LEXER_STATE("sh-string-ansi", ("sh-string-single", "sh-base-escape")),
);

static t_lexer_def		g_sh_text_string_lexer = LEXER_DEF(
	(&g_sh_text_string_lexer_base), "sh-string"
);

static t_lexer_def		g_sh_text_string_single_lexer = LEXER_DEF(
	(&g_sh_text_string_lexer_base), "sh-string-single"
);

static t_lexer_def		g_sh_text_string_ansi_lexer = LEXER_DEF(
	(&g_sh_text_string_lexer_base), "sh-string-ansi"
);

static t_lexer_def		*get_string_state(t_sh_parser *p)
{
	if (SH_T(p)->string == SH(STRING_NORMAL))
		return (&g_sh_text_string_lexer);
	if (SH_T(p)->string == SH(STRING_SIMPLE))
		return (&g_sh_text_string_single_lexer);
	if (SH_T(p)->string == SH(STRING_ANSI))
		return (&g_sh_text_string_ansi_lexer);
	HARD_ASSERT(false);
}

bool					sh_parse_text_string(t_sh_parser *p,
							t_sh_text *dst, bool quoted)
{
	t_lexer_frame	frame;

	ft_lexer_push(&p->t, &frame, get_string_state(p));
	sh_text_push_string(dst, SUB0(), true);
	while (ft_tokenize(&p->t))
		if (SH_T_EQU(p, STRING, STRING_END))
			break ;
		else if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			if (!g_sh_parse_text[SH_T(p)->type](p, dst, true))
				return (false);
		}
		else
			return (sh_parse_error(p, SH_E_ERROR));
	ft_lexer_pop(&p->t, &frame);
	return (true);
	(void)quoted;
}
