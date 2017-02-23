/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_term.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 15:08:16 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/23 18:14:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_TERM_H
# define EDITOR_TERM_H

# include "ft/libft.h"
# include "ft/term.h"

# include "editor.h"

typedef struct s_editor_term			t_editor_term;

/*
** ========================================================================== **
** Render an editor's content to the terminal
*/

/*
** Editor term object
*/
struct			s_editor_term
{
	t_editor_text_listener		text_listener;
	t_editor_cursor_listener	cursor_listener;
	t_term						*term;
	t_editor					*editor;
	t_vec2u						size;
	t_vec2u						scroll;
	t_vector					updates;
};

/*
** Init
*/
void			editor_term_init(t_editor_term *dst, t_term *term,
					t_editor *editor, t_vec2u size);

/*
** Update the terminal
*/
void			editor_render(t_editor_term *t);

/*
** Rerender everything
*/
void			editor_rerender(t_editor_term *t);

#endif
