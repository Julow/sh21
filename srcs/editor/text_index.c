/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_index.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/01 19:13:58 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 12:45:33 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

t_vec2u		editor_rowcol(t_editor const *editor, uint32_t index)
{
	uint32_t const *const	stops = editor->line_stops.data;
	t_vec2u					rc;

	rc.y = 0;
	while (index > 0)
	{
		if (index < stops[rc.y])
			break ;
		index -= stops[rc.y++];
		if (rc.y >= editor->line_stops.length)
			break ;
	}
	rc.x = index;
	return (rc);
}

uint32_t	editor_index(t_editor const *editor, t_vec2u rc)
{
	uint32_t const *const	stops = editor->line_stops.data;
	uint32_t				y;

	y = 0;
	if (rc.y >= editor->line_stops.length)
		return (editor->text.length);
	while (y < rc.y)
		rc.x += stops[y++];
	return (rc.x);
}
