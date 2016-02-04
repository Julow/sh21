/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_syntax_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:44:48 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 00:41:19 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "internal.h"

static t_syntax_color_def const *const	g_syntax_color_def[] = {
	&g_syntax_color_utils,
	&g_syntax_color_sh,
	&g_syntax_color_xml,
};

t_parser const	*load_syntax_color(t_sub name)
{
	static t_hmap				*parser_map = NULL;
	t_syntax_color_def const	*def;
	t_parser const				*tmp;
	uint32_t					i;
	uint32_t					j;

	if (parser_map == NULL)
		parser_map = ft_hmapnew(20, &ft_djb2);
	else if ((tmp = ft_hmapget(parser_map, name).value) != NULL)
		return (tmp);
	i = 0;
	while (i < ARRAY_LEN(g_syntax_color_def))
	{
		def = g_syntax_color_def[i];
		if (SUB_EQU(def->name, name))
		{
			j = 0;
			while (j < def->depend.length)
				if (load_syntax_color(
					*(t_sub*)VECTOR_GET(def->depend, j++)) == NULL)
					return (NULL);
			if (!build_parser(parser_map, &def->def))
				break ;
			if (def->main_parser.length == 0)
				return ((void*)1); // lol NULL2
			return (ft_hmapget(parser_map, def->main_parser).value);
		}
		i++;
	}
	return (NULL);
}
