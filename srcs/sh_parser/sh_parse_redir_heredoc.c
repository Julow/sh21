/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_redir_heredoc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/13 12:17:06 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/14 15:21:38 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "p_sh_parser.h"

#include <stdlib.h>

static t_lexer_def		g_sh_redir_heredoc_text_lexer = LEXER_DEF(
	(&g_sh_lexer_base), "sh-base-text",
);

static t_lexer_def		g_sh_redir_heredoc_string_lexer = LEXER_DEF(
	(&g_sh_lexer_base), "heredoc-string",

	LEXER_STATE("heredoc-string", ("sh-base-text"),
		LEXER_T("\"", T(STRING, .string=SH(STRING_END))),
		LEXER_T("\\\"", T(ESCAPED, '"')),
	),
);

static t_lexer_def		g_sh_redir_heredoc_string_single_lexer = LEXER_DEF(
	(&g_sh_text_string_lexer_base), "sh-string-single",
);

static t_lexer_def		g_sh_redir_heredoc_string_ansi_lexer = LEXER_DEF(
	(&g_sh_text_string_lexer_base), "sh-string-ansi",
);

static bool		parse_heredoc_string(t_sh_parser *p, t_sh_text *dst)
{
	t_lexer_frame		frame;

	if (SH_T(p)->string == SH(STRING_NORMAL))
		ft_lexer_push(&p->t, &frame, &g_sh_redir_heredoc_string_lexer);
	else if (SH_T(p)->string == SH(STRING_SIMPLE))
		ft_lexer_push(&p->t, &frame, &g_sh_redir_heredoc_string_single_lexer);
	else if (SH_T(p)->string == SH(STRING_ANSI))
		ft_lexer_push(&p->t, &frame, &g_sh_redir_heredoc_string_ansi_lexer);
	else
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	while (true)
		if (!ft_tokenize(&p->t))
			return (sh_parse_error_unterminated(p, SH_E_UNTERMINATED_STRING));
		else if (SH_T(p)->type == SH(STRING))
			break ;
		else if (g_sh_parse_text[SH_T(p)->type] == NULL)
			return (sh_parse_error(p, SH_E_ERROR));
		else if (!g_sh_parse_text[SH_T(p)->type](p, dst, true))
			return (false);
	ft_lexer_pop(&p->t, &frame);
	return (true);
}

static bool		parse_heredoc_right(t_sh_parser *p, t_sh_text *dst)
{
	t_lexer_frame		frame;
	bool				r;

	ft_lexer_push(&p->t, &frame, &g_sh_redir_heredoc_text_lexer);
	sh_ignore_spaces(p);
	r = true;
	while (r && ft_tokenize(&p->t))
	{
		if (SH_T(p)->type == SH(SPACE))
			break ;
		else if (SH_T(p)->type == SH(STRING))
			r = parse_heredoc_string(p, dst);
		else if (g_sh_parse_text[SH_T(p)->type] != NULL)
			r = g_sh_parse_text[SH_T(p)->type](p, dst, false);
		else
			r = sh_parse_error(p, SH_E_UNEXPECTED);
	}
	ft_lexer_pop(&p->t, &frame);
	return (r);
}

static bool		full_text(t_sh_parser *p, t_sh_text const *text, uint32_t *flags)
{
	t_sh_token const	*t;

	t = VECTOR_IT(text->tokens);
	while (VECTOR_NEXT(text->tokens, t))
	{
		if ((t->type & ~SH_F_T_QUOTED) != SH_T_STRING)
		{
			ASSERT(!"Heredoc word not full text");
			return (sh_parse_error(p, SH_E_ERROR));
		}
		if (t->type & SH_F_T_QUOTED)
			*flags |= SH_HEREDOC_QUOTED;
	}
	return (true);
}

static void		put_heredoc(t_sh_parser *p, t_sh_parser_heredoc *heredoc)
{
	t_sh_parser_heredoc	**dst;

	dst = &p->heredoc_lst;
	while (*dst != NULL)
		dst = &(*dst)->next;
	*dst = heredoc;
}

bool			sh_parse_redir_heredoc(t_sh_parser *p, t_sh_redir *dst)
{
	t_sh_text			right_text;
	uint32_t			flags;
	t_sh_parser_heredoc	*heredoc;

	flags = 0;
	dst->type = SH_REDIR_STRING;
	if (SH_T(p)->heredoc_strip)
		flags |= SH_HEREDOC_STRIP;
	right_text = SH_TEXT();
	if (!parse_heredoc_right(p, &right_text)
		|| !full_text(p, &right_text, &flags))
	{
		sh_destroy_text(&right_text);
		return (false);
	}
	dst->here_string = NEW(t_sh_text);
	*dst->here_string = SH_TEXT();
	heredoc = MALLOC(sizeof(t_sh_parser_heredoc) + right_text.text.length);
	*heredoc = (t_sh_parser_heredoc){
		NULL,
		SUB_DST(ENDOF(heredoc), DSTR_SUB(right_text.text)),
		flags,
		dst->here_string,
	};
	sh_destroy_text(&right_text);
	put_heredoc(p, heredoc);
	return (true);
}
