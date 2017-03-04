/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 19:13:22 by jaguillo          #+#    #+#             */
/*   Updated: 2017/03/02 17:38:26 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft/ft_dstr.h"
# include "ft/ft_in.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "ft/sumset.h"

typedef t_vec2u							t_editor_sel;
typedef struct s_editor_text_event		t_editor_text_event;
typedef struct s_editor_text_listener	t_editor_text_listener;
typedef struct s_editor_cursor_listener	t_editor_cursor_listener;
typedef struct s_editor_in				t_editor_in;
typedef struct s_editor					t_editor;

/*
** ========================================================================== **
** Editor
*/

/*
** editor_sel object
** -
** Represent a range of text (a selection)
** -
** x				=> First point (cursor position)
** y				=> Second point (may be < than 'x')
** -
** EDITOR_SEL(X, Y)			Constructor
** EDITOR_SEL_NORM(SEL+)	Normalize (make 'sel' positive)
** EDITOR_SEL_BEGIN(SEL+)	Begin point (left-most point)
** EDITOR_SEL_END(SEL+)		End point (right most point)
*/

# define EDITOR_SEL(X, Y)		VEC2U((X), (Y))

# define EDITOR_SEL_NORM(S)		N_VEC2U((S).x, (S).y)

# define EDITOR_SEL_BEGIN(S)	MIN((S).x, (S).y)
# define EDITOR_SEL_END(S)		MAX((S).x, (S).y)

# define EDITOR_SEL_LENGTH(S)	DIFF((S).x, (S).y)

/*
** Text listener's param
** -
** sel			=> Selection in the old text (normalized)
** sel_begin
** sel_end		=> Begin/end point (line/col) (computed from 'sel')
** text			=> New text (that replace the text under 'sel')
** line_count	=> Number of line in 'text'
*/
struct			s_editor_text_event
{
	t_editor_sel	sel;
	t_vec2u			sel_begin;
	t_vec2u			sel_end;
	t_sub			text;
	uint32_t		line_count;
};

/*
** on_change	=> Called when some text is inserted/removed/replaced
** 					Text modifications occur after this is called
** 				| l			=> listener instance
** 				| event		=> event object
** 				| batch		=> true if listeners will be called again
*/
struct			s_editor_text_listener
{
	void			(*on_change)(t_editor_text_listener *l,
							t_editor_text_event const *event, bool batch);
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
						t_editor_sel const *new_cursors, uint32_t len, bool batch);
};

# define EDITOR_CURSOR_LISTENER(C)	((t_editor_cursor_listener){V(C)})

/*
** Editor object
** -
** Handle text and cursors
** Propagate changes through listeners
** -
** text			=> text // TODO: rope
** lines		=> line lengths
** cursors		=> vector of cursor (editor_sel) (sorted by position)
** -
** EDITOR()				Constructor
** EDITOR_LINE(E, I)	Return the length of the line 'I'
** EDITOR_CURSOR(E, I)	Return the cursor 'I'
*/
struct			s_editor
{
	t_dstr			text;
	t_sumset		lines;
	t_vector		cursors;
	t_vector		text_listeners;
	t_vector		cursor_listeners;
};

# define EDITOR()	((t_editor){DSTR0(), SUMSET(), VECTORC(((t_editor_sel[]){{0,0}})), VECTOR(void*), VECTOR(void*)})

# define EDITOR_LINE(E,I)	(ft_sumset_get(&(E)->lines, (I)).y)

# define EDITOR_CURSOR(E,I)	(VGETC(t_editor_sel, (E)->cursors, (I)))

/*
** Init the editor
*/
void			editor_init(t_editor *dst);

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
** 'c' does not need to be sorted, dupplicated cursors are removed
*/
void			editor_set_cursors(t_editor *editor, t_editor_sel const *c,
					uint32_t cursor_count, bool batch);

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
** Compute the pos for 'linecol'
*/
uint32_t		editor_getindex(t_editor const *editor, t_vec2u linecol);

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

#endif
