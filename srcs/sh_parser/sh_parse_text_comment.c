/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_comment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/08 17:58:02 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/08 18:01:09 by jaguillo         ###   ########.fr       */
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

	ft_lexer_push(&p->l, &frame, &g_sh_text_comment_lexer);
	while (true)
	{
		if (!ft_lexer_ahead(&p->l, NULL, V(&t))
			|| t->type == SH_PARSE_T_COMMENT)
			break ;
		if (!ft_lexer_next(&p->l))
			ASSERT(false);
	}
	ft_lexer_pop(&p->l, &frame);
	return (true);
	(void)dst;
	(void)quoted;
}
