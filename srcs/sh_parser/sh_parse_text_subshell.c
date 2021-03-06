/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_subshell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 18:15:20 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/11 15:14:16 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static t_lexer_def const	g_sh_text_subshell_lexer_base = LEXER_DEF(
	(&g_sh_lexer_base), NULL,

	LEXER_STATE("sh-subshell-brace", ("sh-base-cmd"),
		LEXER_T(")", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL))),
	),

	LEXER_STATE("sh-subshell-backquote", ("sh-base-cmd"),
		LEXER_T("\\`", T(SUBSHELL, .subshell=SH(SUBSHELL_BACKQUOTE_REV))),
		LEXER_T("`", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL))),
	),

	LEXER_STATE("sh-subshell-backquote-rev", ("sh-base-cmd"),
		LEXER_T("`", T(SUBSHELL, .subshell=SH(SUBSHELL_BACKQUOTE))),
		LEXER_T("\\`", T(COMPOUND_END, .compound_end=SH(COMPOUND_SUBSHELL))),
	),
);

static t_lexer_def			g_sh_text_subshell_brace_lexer = LEXER_DEF(
	(&g_sh_text_subshell_lexer_base), "sh-subshell-brace",
);

static t_lexer_def			g_sh_text_subshell_backquote_lexer = LEXER_DEF(
	(&g_sh_text_subshell_lexer_base), "sh-subshell-backquote",
);

static t_lexer_def			g_sh_text_subshell_backquote_rev_lexer = LEXER_DEF(
	(&g_sh_text_subshell_lexer_base), "sh-subshell-backquote-rev",
);

static t_lexer_def		*get_subshell_state(t_sh_parser *p)
{
	if (SH_T(p)->subshell == SH(SUBSHELL_BRACE))
		return (&g_sh_text_subshell_brace_lexer);
	if (SH_T(p)->subshell == SH(SUBSHELL_BACKQUOTE))
		return (&g_sh_text_subshell_backquote_lexer);
	if (SH_T(p)->subshell == SH(SUBSHELL_BACKQUOTE_REV))
		return (&g_sh_text_subshell_backquote_rev_lexer);
	HARD_ASSERT(false);
}

static bool				unterminated_error(t_sh_parser *p, uint32_t t)
{
	if (t == SH(SUBSHELL_BRACE))
		sh_parse_error_unterminated(p, SH_E_UNTERMINATED_SUBST_SUBSHELL);
	else if (t == SH(SUBSHELL_BACKQUOTE))
		sh_parse_error_unterminated(p, SH_E_UNTERMINATED_SUBST_BACKQUOTE);
	else if (t == SH(SUBSHELL_BACKQUOTE_REV))
		sh_parse_error_unterminated(p, SH_E_UNTERMINATED_SUBST_BACKQUOTE_REV);
	return (false);
}

bool			sh_parse_text_subshell(t_sh_parser *p,
					t_sh_text *dst, bool quoted)
{
	uint32_t const			subshell_type = SH_T(p)->subshell;
	t_lexer_frame			frame;
	t_sh_compound *const	cmd = NEW(t_sh_compound);
	bool					r;

	ft_lexer_push(&p->t, &frame, get_subshell_state(p));
	if ((r = sh_parse_compound(p, cmd, true)
			&& (!p->t.eof || unterminated_error(p, subshell_type))
			&& (SH_T(p)->type == SH_PARSE_T_COMPOUND_END || sh_parse_error(p, SH_E_ERROR))
			&& (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_SUBSHELL)
		))
		sh_text_push(dst, SUB0(), SH_TOKEN(SUBSHELL, .cmd=cmd), quoted);
	else
		free(cmd);
	ft_lexer_pop(&p->t, &frame);
	return (r);
}
