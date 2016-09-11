/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_do_clause.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 21:19:03 by juloo             #+#    #+#             */
/*   Updated: 2016/09/11 15:20:39 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

bool			sh_parse_do_clause(t_sh_parser *p, t_sh_compound *dst)
{
	return (SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_DO)
		&& (sh_ignore_newlines(p)
				|| sh_parse_error_unterminated(p, SH_E_UNTERMINATED_DO))
		&& sh_parse_compound(p, dst, true)
		&& (((!p->t.eof
				|| sh_parse_error_unterminated(p, SH_E_UNTERMINATED_DO))
			&& SH_T_EXCEPT(p, COMPOUND_END, COMPOUND_DONE))
			|| (sh_destroy_compound(dst), false)));
}
