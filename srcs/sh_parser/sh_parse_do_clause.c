/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_do_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 21:19:03 by juloo             #+#    #+#             */
/*   Updated: 2016/08/13 18:36:20 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static bool		parse_trailing_redirs(t_sh_parser *p)
{
	while (ft_lexer_next(&p->l))
	{
		if (SH_T(p)->type == SH(REDIR))
			ASSERT(!"TODO: redirs");
		break ;
	}
	return (true);
}

bool			sh_parse_do_clause(t_sh_parser *p, t_sh_compound *dst)
{
	return ((!p->l.eof || sh_parse_error(p, SH_E_EOF))
		&& ((SH_T_EQU(p, COMPOUND_END)
			&& SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_DO)
				|| sh_parse_error(p, SH_E_UNEXPECTED))
		&& (sh_ignore_newlines(p) || sh_parse_error(p, SH_E_EOF))
		&& sh_parse_compound(p, dst, true)
		&& (((!p->l.eof || (TRACE(), sh_parse_error(p, SH_E_EOF)))
				&& ((SH_T_EQU(p, COMPOUND_END)
						&& SH_T(p)->compound_end == SH(COMPOUND_DONE))
					|| sh_parse_error(p, SH_E_UNEXPECTED))
				&& parse_trailing_redirs(p))
			|| (sh_destroy_compound(dst), false)));
}
