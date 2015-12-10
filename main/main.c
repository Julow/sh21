/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2015/12/10 01:14:41 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/term.h"
#include "ft/ft_printf.h"

int				main(void)
{
	t_term *const	term = ft_tinit(1, TERM_RAW | TERM_LINE);
	// t_term *const	term = ft_tinit(1, TERM_RAW | TERM_FULLSCREEN);
	// t_term *const	term = ft_tinit(1, TERM_RAW);
	char			c;
	int				lol;
	int				i;

	if (term->flags & TERM_USE_DEFAULT)
		ft_dprintf(2, "Warning: Invalid $TERM value: Use default: %s%n",
			TERM_DEFAULT_TERM);
	ft_trestore(term, true);
	lol = 0;
	while ((c = ft_getchr()) != 'q')
	{
		if (c == 'a')
			lol++;
		else if (c == 'b')
			lol--;
		else
			continue ;
		ft_tclear(term);
		i = -1;
		while (++i < lol)
			ft_fprintf(&term->out, "%-3d:: %.*#\n\r", i, lol);
		ft_flush(&term->out);
	}
	ft_trestore(term, false);
	return (0);
}
