/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 19:13:22 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/11 15:43:45 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft/ft_dstr.h"
# include "ft/ft_in.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"

typedef struct s_editor_sel				t_editor_sel;
typedef struct s_editor_text_listener	t_editor_text_listener;
typedef struct s_editor_cursor_listener	t_editor_cursor_listener;
typedef struct s_editor_in				t_editor_in;
typedef struct s_editor					t_editor;

/*
** ========================================================================== **
** Editor
*/

/*
** Represent a range of text (a selection)
** -
** pos				=> position in text
** sel				=> length of the selection (can be negative)
** -
** EDITOR_SEL(POS, SEL)		Constructor
** EDITOR_SEL_NORM(SEL+)	Normalize (make 'sel' positive)
*/
struct			s_editor_sel
{
	uint32_t		pos;
	int32_t			sel;
};

# define EDITOR_SEL(POS, SEL)	((t_editor_sel){(POS), (SEL)})

# define EDITOR_SEL_NORM(S)		((S).sel < 0 ? _EDITOR_SEL_NORM(S) : (S))

/*
** on_change	=> Called when some text is inserted/removed/replaced
** 					Text modifications occur after this is called
** 				| l			=> listener instance
** 				| sel		=> range of text that is replaced (normalized)
** 				| new_text	=> inserted text
** 				| batch		=> true if listeners will be called again
*/
struct			s_editor_text_listener
{
	void			(*on_change)(t_editor_text_listener *l, t_editor_sel sel,
						t_sub new_text, bool batch);
};

# define EDITOR_TEXT_LISTENER(C)	((t_editor_text_listener){V(C)})

/*
** on_change	=> Called when cursors are set (moved and/or added/removed)
** 				| l				=> listener instance
** 				| new_cursors	=> new array of cursors (sorted by position)
** 				| len			=> length of 'new_cursors'
*/
struct			s_editor_cursor_listener
{
	void			(*on_change)(t_editor_cursor_listener *l,
						t_editor_sel const *new_cursors, uint32_t len);
};

# define EDITOR_CURSOR_LISTENER(C)	((t_editor_cursor_listener){V(C)})

/*
** Editor object
** -
** Handle text and cursors
** Propagate changes through listeners
** -
** text			=> text // TODO: rope
** lines		=> line lengths // TODO: efficient prefix sum
** cursors		=> vector of cursor (editor_sel) (sorted by position)
*/
struct			s_editor
{
	t_dstr			text;
	t_vector		lines;
	t_vector		cursors;
	t_vector		text_listeners;
	t_vector		cursor_listeners;
};

# define EDITOR()	((t_editor){DSTR0(), VECTOR(uint32_t), VECTOR(t_editor_sel), VECTOR(void*), VECTOR(void*)})

/*
** Write some text at an arbitrary position
** Overwriting characters in 'sel'
** Notify text listeners
** 'batch' is passed to the listeners, if it is true,
** 	there must be an other call before any other editor operation
*/
void			editor_write(t_editor *editor, t_editor_sel sel, t_sub text,
					bool batch);

/*
** Put text after each cursors
** Them move them
** Notify text listeners and cursor listeners
*/
void			editor_put(t_editor *editor, t_sub text, bool batch);

/*
** Replace current cursors
** Notify cursor listeners
*/
void			editor_set_cursors(t_editor *editor,
					t_editor_sel const *c, uint32_t len);

/*
** Register some listeners
*/
void			editor_register_listener(t_editor *editor,
					t_editor_text_listener *text_l,
					t_editor_cursor_listener *cursor_l);

/*
** Compute the line/column numbers for 'pos'
*/
t_vec2u			editor_getpos(t_editor const *editor, uint32_t pos);

/*
** Stream that read the content of an editor
*/
struct			s_editor_in
{
	t_in			in;
	t_editor const	*editor;
	uint32_t		line;
};

/*
** Initialize an editor_in
*/
void			editor_read(t_editor const *editor, t_editor_in *dst);

/*
** -
*/

# define _EDITOR_SEL_NORM(S)	(EDITOR_SEL((S).pos + (S).sel, -(S).sel))

#endif
