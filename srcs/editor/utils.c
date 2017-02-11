/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 15:21:11 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/11 17:43:40 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_vec2u			editor_getpos(t_editor const *editor, uint32_t pos)
{
	uint32_t				i;
	uint32_t const *const	line_lengths = editor->lines.data;

	i = 0;
	while (line_lengths[i] < pos)
		pos -= line_lengths[i++];
	if (line_lengths[i] == pos && i < editor->lines.length - 1)
		return (VEC2U(i + 1, 0));
	return (VEC2U(i, pos));
}
