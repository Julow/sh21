/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 12:55:19 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/15 16:24:54 by jaguillo         ###   ########.fr       */
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

typedef struct s_editor			t_editor;

/*
** ========================================================================== **
** Editor
*/

struct		s_editor
{
	t_dstr		text;
	int32_t		cursor;
	int32_t		sel;
	t_bst		bindings;
	t_list		clipboard;
	void		*user;
};

/*
** Return 1 if N is positive -1 otherwise
*/
# define INT_NORM(N)	(((N) < 0) ? -1 : 1)

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

t_range		ft_word_range(t_sub sub, int32_t at, bool subword);

#endif
