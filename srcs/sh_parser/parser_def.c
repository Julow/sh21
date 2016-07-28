/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_def.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 18:21:47 by juloo             #+#    #+#             */
/*   Updated: 2016/07/28 18:29:58 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_vector.h"
#include "ft/parser_def.h"

#include "p_sh_parser.h"

static t_vector const	g_sh_parser = VECTORC(((t_parser_def const[]){

}));

t_parser const	*sh_load_parser(void)
{
	static t_parser const	*sh_parser = NULL;
	static t_hmap			*map = NULL;

	if (sh_parser == NULL)
	{
		map = ft_hmapnew(10, &ft_djb2);
		if (!build_parser(map, &g_sh_parser)
			|| (sh_parser = ft_hmapget(map, SUBC("sh-cmd")).value) == NULL)
			HARD_ASSERT(!"Fail building sh parser");
	}
	return (sh_parser);
}
