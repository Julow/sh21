/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 12:55:19 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 18:31:03 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft/ft_bst.h"
# include "ft/ft_dstr.h"
# include "ft/ft_list.h"
# include "ft/ft_out.h"
# include "ft/getkey.h"
# include "ft/libft.h"
# include "ft/range.h"
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
** Execute 'key' binding
*/
bool		editor_key(t_editor *editor, t_key key);

/*
** Put the editor text to 'out'
*/
void		editor_put(t_editor const *editor, t_out *out);

/*
** Add a key binding
** -
** See editor_key
*/
void		editor_bind(t_editor *editor, t_key key,
				bool (*f)(t_editor*, uint32_t), uint32_t flags);

/*
** ========================================================================== **
** Editor actions
*/

# define EDITOR_SEL_PRIORITY		128

void		editor_set_cursor(t_editor *editor, uint32_t cursor, int32_t sel);

t_range		ft_word_range(t_sub sub, int32_t at, bool subword);

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
