/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_heredoc_lst.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/14 11:50:29 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/14 15:21:42 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static t_lexer_def const	g_sh_heredoc_lexer_base = LEXER_DEF(
	(&g_sh_lexer_base), NULL,

	LEXER_STATE("heredoc-base", ("sh-base"),
		LEXER_T("\n", T(COMPOUND_END, .compound_end=SH(COMPOUND_NEWLINE))),
	),
);

static t_lexer_def			g_sh_heredoc_lexer = LEXER_DEF(
	(&g_sh_heredoc_lexer_base), "heredoc",
	LEXER_STATE("heredoc", ("heredoc-base", "sh-base-subst")),
);

static t_lexer_def			g_sh_heredoc_quoted_lexer = LEXER_DEF(
	(&g_sh_heredoc_lexer_base), "heredoc-quoted",
	LEXER_STATE("heredoc-quoted", ("heredoc-base")),
);

// TODO: fix:
// > cat << $(ls)
// > $(ls)

static void		read_heredoc_text(t_sh_parser *p, t_sh_parser_heredoc *h)
{
	while (!p->t.eof && ft_tokenize(&p->t))
	{
		if (SH_T(p)->type == SH(TEXT) && SUB_EQU(p->t.token_str, h->end))
		{
			if (!ft_tokenize(&p->t) || SH_T(p)->type != SH(COMPOUND_END))
				sh_text_push_string(h->dst, h->end, true);
			else
				return ;
		}
		while (true)
		{
			if (SH_T(p)->type == SH(COMPOUND_END))
				break ;
			else if (g_sh_parse_text[SH_T(p)->type] != NULL)
				g_sh_parse_text[SH_T(p)->type](p, h->dst, true);
			else
				ASSERT(!"Unknown token");
			if (!ft_tokenize(&p->t))
				break ;
		}
	}
}

bool			sh_parse_heredoc_lst(t_sh_parser *p)
{
	t_sh_parser_heredoc		*h;
	t_sh_parser_heredoc		*tmp;
	t_lexer_frame			frame;

	TRACE();
	h = p->heredoc_lst;
	p->heredoc_lst = NULL;
	while (h != NULL)
	{
		TRACE();
		ft_lexer_push(&p->t, &frame, (h->flags & SH_HEREDOC_QUOTED) ?
				&g_sh_heredoc_quoted_lexer : &g_sh_heredoc_lexer);
		read_heredoc_text(p, h);
		ft_lexer_pop(&p->t, &frame);
		tmp = h->next;
		free(h);
		h = tmp;
	}
	return (true);
}
