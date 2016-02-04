/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 12:55:19 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 14:33:59 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft/ft_bst.h"
# include "ft/ft_dstr.h"
# include "ft/ft_list.h"
# include "ft/ft_out.h"
# include "ft/ft_vector.h"
# include "ft/getkey.h"
# include "ft/libft.h"
# include "ft/spanlist.h"

typedef struct s_editor			t_editor;
typedef struct s_style			t_style;

/*
** Return 1 if N is positive -1 otherwise
** TODO: move
*/
# define INT_NORM(N)	(((N) < 0) ? -1 : 1)

/*
** ========================================================================== **
** Editor
*/

struct		s_editor
{
	t_dstr		text;
	t_vector	line_stops;
	t_spanlist	spans;
	uint32_t	cursor;
	int32_t		sel;
	t_bst		bindings;
	t_list		clipboard;
	uint32_t	extra_mods;
	void		*user;
};

/*
** Init editor and default bindings
*/
void		editor_init(t_editor *editor);

/*
** Execute binding
*/
bool		editor_key(t_editor *editor, t_key key);

/*
** Put the editor text to 'out'
*/
void		editor_out(t_editor const *editor, t_out *out);

/*
** Add a key binding
** -
** 'callback' is of type:
**  bool (*)(void *self, t_editor *editor, t_key key)
** -
** If 'key.c' is -1, it match any key with the same modifier
** -
** If a binding return false, the next matching binding is executed
**  (highest priority match first)
** -
** Default bindings have all a priority of 0
*/
void		editor_bind(t_editor *editor, t_key key,
				t_callback callback, int32_t priority);

/*
** Return the row/column index for the text index 'index'
*/
t_vec2u		editor_rowcol(t_editor const *editor, uint32_t index);

/*
** Return the text index for the row/col rc
*/
uint32_t	editor_index(t_editor const *editor, t_vec2u rc);

/*
** ========================================================================== **
** Editor actions
*/

/*
** Return the length of the line Y
*/
# define EDITOR_LINE(E, Y)	(((uint32_t const*)((E)->line_stops.data))[(Y)])

/*
** Override a span of text
*/
void		editor_write(t_editor *editor, t_vec2u span, t_sub str);

/*
** Set cursor
*/
void		editor_set_cursor(t_editor *editor, uint32_t cursor, int32_t sel);

/*
** -
*/

t_vec2u		ft_word_range(t_sub sub, uint32_t at, bool subword);

# define EDITOR_SEL_PRIORITY		128

/*
** ========================================================================== **
** Style
*/

struct		s_style
{
	uint32_t	foreground;
	uint32_t	background;
	uint32_t	styles;
};

# define STYLE(F,B,S)			((t_style){(F), (B), (S)})

# define S_BLACK				0
# define S_RED					1
# define S_GREEN				2
# define S_YELLOW				3
# define S_BLUE					4
# define S_MAGENTA				5
# define S_CYAN					6
# define S_WHITE				7

# define S_LIGHT(COLOR)			((COLOR) | STYLE_F_LIGHT)
# define S_EXTENDED(COLOR)		((COLOR) | STYLE_F_EXTENDED)

# define S_UNDERLINE			(1 << 0)
# define S_REVERSED				(1 << 1)
# define S_BOLD					(1 << 2)

# define STYLE_F_EXTENDED		(1 << 31)
# define STYLE_F_LIGHT			(1 << 30)

#endif
