/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_do_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 21:19:03 by juloo             #+#    #+#             */
/*   Updated: 2016/08/16 00:06:55 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

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
					|| sh_parse_error(p, SH_E_UNEXPECTED)))
			|| (sh_destroy_compound(dst), false)));
}
