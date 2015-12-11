/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 12:55:19 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/11 17:17:15 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include "ft/ft_bst.h"
# include "ft/ft_dstr.h"
# include "ft/ft_out.h"
# include "ft/getkey.h"
# include "ft/libft.h"

typedef struct s_editor			t_editor;

/*
** ========================================================================== **
** Editor
** TODO: multi line
** TODO: multi cursor
** TODO: sequenced binding (like ctrl+K,ctrl+C)
** TODO: next matching binding on binding returning false
*/

struct		s_editor
{
	t_dstr		text;
	uint32_t	cursor;
	uint32_t	sel;
	t_bst		bindings;
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
void		editor_bind(t_editor *editor,
				t_key key, bool (*f)(t_editor*, t_key));

#endif
