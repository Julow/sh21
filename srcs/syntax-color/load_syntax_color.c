/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_syntax_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:44:48 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/09 15:31:26 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_hmap.h"
#include "internal.h"

extern t_vector const	g_syntaxes_color_def;

t_syntax_color	*load_syntax_color(t_sub name)
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
	while (i < g_syntaxes_color_def.length)
	{
		def = *(t_syntax_color_def const**)VECTOR_GET(g_syntaxes_color_def, i);
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
