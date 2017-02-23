/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_editor_term.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 18:13:32 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/23 18:16:47 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_EDITOR_TERM_H
# define P_EDITOR_TERM_H

# include "editor_term.h"

typedef struct s_editor_term_update		t_editor_term_update;

/*
** ========================================================================== **
*/

/*
** Update
** -
** REDRAW		=> Redraw a part of a line (line, col, length)
** INSERT_LINE	=> Insert lines (line, count)
** DELETE_LINE	=> Delete lines (line, count)
** SCROLL		=> Scroll (x/y)
*/
struct			s_editor_term_update
{
	enum {
		EDITOR_TERM_UPDATE_REDRAW,
		EDITOR_TERM_UPDATE_INSERT_LINE,
		EDITOR_TERM_UPDATE_DELETE_LINE,
		EDITOR_TERM_UPDATE_SCROLL,
	}				type;
	union {
		t_vec3u			redraw;
		t_vec2u			insert_line;
		t_vec2u			delete_line;
		t_vec2i			scroll;
	};
};

/*
** Push a redraw update
*/
void			editor_term_redraw(t_editor_term *t, t_vec2u pos, uint32_t len);

/*
** Push a insert/delete line update
** INSERT_LINE if count > 0 otherwise DELETE_LINE
*/
void			editor_term_line(t_editor_term *t, uint32_t line,
					int32_t count);

#endif
