/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_viewport.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 17:52:34 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/06 22:15:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_VIEWPORT_H
# define EDITOR_VIEWPORT_H

# include "ft/ft_vector.h"
# include "ft/libft.h"

# include "editor.h"

typedef struct s_editor_viewport			t_editor_viewport;
typedef struct s_editor_viewport_event		t_editor_viewport_event;
typedef struct s_editor_viewport_listener	t_editor_viewport_listener;

/*
** ========================================================================== **
** Viewport
*/

/*
** Viewport event
** -
** EDITOR_VIEWPORT_EVENT_SCROLL			=> Scroll changed
** EDITOR_VIEWPORT_EVENT_SIZE			=> Viewport size changed
** EDITOR_VIEWPORT_EVENT_LINE_WRAP		=> LINE_WRAP flag changed
** EDITOR_VIEWPORT_EVENT_LINE_HEIGHTS	=> some line height changed
** 									(not called for SCROLL, SIZE or LINE_WRAP)
*/
struct			s_editor_viewport_event
{
	enum {
		EDITOR_VIEWPORT_EVENT_SCROLL,
		EDITOR_VIEWPORT_EVENT_SIZE,
		EDITOR_VIEWPORT_EVENT_LINE_WRAP,
		EDITOR_VIEWPORT_EVENT_LINE_HEIGHTS,
	}				type;
	union {
		t_vec2u			old_scroll;
		t_vec2u			old_size;
		t_vector		old_heights;
	};
};

/*
** Viewport listener object
** -
** EDITOR_VIEWPORT_LISTENER(ON_CHANGE)	Constructor
*/
struct			s_editor_viewport_listener
{
	void			(*on_change)(t_editor_viewport_listener *l,
						t_editor_viewport_event const *e);
};

# define EDITOR_VIEWPORT_LISTENER(F)	((t_editor_viewport_listener){(F)})

/*
** Viewport object
** -
** scroll			=> Scroll position (col/line)
** size				=> Viewport size (col/line)
** line_heights		=> The height of the visible lines (if LINE_WRAP is enabled)
** flags			=>
** 		EDITOR_LINE_WRAP	Enable line wrap,
** 							lines longer than 'size.x' span on multiple lines
*/
struct			s_editor_viewport
{
	t_editor					*editor;
	t_vec2u						scroll;
	t_vec2u						size;
	t_vector					line_heights;
	uint32_t					flags;
	t_vector					listeners;
	t_editor_text_listener		text_listener;
	t_editor_cursor_listener	cursor_listener;
};

# define EDITOR_LINE_WRAP		(1 << 0)

/*
** Init the viewport object
*/
void			editor_viewport_init(t_editor *e, t_editor_viewport *dst);

/*
** Set the viewport size
** Call the listeners
*/
void			editor_set_size(t_editor_viewport *v, t_vec2u size);

/*
** Set the viewport scroll
** scroll.x should be 0 if LINE_WRAP is enabled
** Call the listeners
*/
void			editor_set_scroll(t_editor_viewport *v, t_vec2u scroll);

/*
** Set line wrap
** Call the listeners
** Automatically scroll to column 0 and recompute 'line_heights'
*/
void			editor_set_linewrap(t_editor_viewport *v, bool enabled);

/*
** Register a viewport listener
*/
void			editor_viewport_register_listener(t_editor_viewport *v,
					t_editor_viewport_listener *l);

#endif
