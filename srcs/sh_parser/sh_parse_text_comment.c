/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_comment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 17:58:02 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/09 13:04:17 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static t_lexer_def		g_sh_text_comment_lexer = LEXER_DEF(
	(&g_sh_lexer_base), "sh-comment",

	LEXER_STATE("sh-comment", ("sh-base"),
		LEXER_T("\n", T(COMMENT)),
	),
);

bool			sh_parse_text_comment(t_sh_parser *p,
					t_sh_text *dst, bool quoted)
{
	t_lexer_frame			frame;
	t_sh_parse_token const	*t;

	ft_lexer_push(&p->t, &frame, &g_sh_text_comment_lexer);
	while (true)
	{
		if (!ft_tokenize_ahead(&p->t, NULL, V(&t))
			|| t->type == SH_PARSE_T_COMMENT)
			break ;
		if (!ft_tokenize(&p->t))
			ASSERT(false);
	}
	ft_lexer_pop(&p->t, &frame);
	return (true);
	(void)dst;
	(void)quoted;
}
