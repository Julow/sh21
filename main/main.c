/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2015/12/10 19:11:46 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/term.h"
#include "ft/getkey.h"
#include "ft/ft_printf.h"

int				main(void)
{
	t_term *const	term = ft_tinit(1, TERM_RAW | TERM_LINE);
	// t_term *const	term = ft_tinit(1, TERM_RAW | TERM_FULLSCREEN);
	// t_term *const	term = ft_tinit(1, TERM_RAW);
	t_key			key;

	if (term->flags & TERM_USE_DEFAULT)
		ft_dprintf(2, "Warning: Invalid $TERM value: Use default: %s%n",
			TERM_DEFAULT_TERM);
	ft_trestore(term, true);
	while (true)
	{
		key = ft_getkey(0);
		ft_tclear(term);
		ft_fprintf(&term->out, "%100.2(Key: %3d '%c' %.5(a)Mods: %.4b (AC.S\\))%n", key.c,
			IS(key.c, IS_PRINT) ? key.c : 0, key.mods);
		if (key.c == 'q' && key.mods == 0)
			break ;
	}
	ft_trestore(term, false);
	return (0);
}
