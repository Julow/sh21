/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 18:13:19 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/23 18:26:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_editor_term.h"

void			editor_term_redraw(t_editor_term *t, t_vec2u pos, uint32_t len)
{
	t_editor_term_update *const	update = ft_vpush(&t->updates, NULL, 1);

	update->type = EDITOR_TERM_UPDATE_REDRAW;
	update->redraw = VEC3U_2(pos, len);
}

void			editor_term_line(t_editor_term *t, uint32_t line,
					int32_t count)
{
	if (count == 0)
		return ;
	*(t_editor_term_update*)ft_vpush(&t->updates, NULL, 1) = (count > 0)
			? (t_editor_term_update){EDITOR_TERM_UPDATE_INSERT_LINE,
				.insert_line = VEC2U(line, count)}
			: (t_editor_term_update){EDITOR_TERM_UPDATE_DELETE_LINE,
				.delete_line = VEC2U(line, -count)};
}
