/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 11:48:17 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/17 20:48:15 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_BINDINGS_H
# define EDITOR_BINDINGS_H

# include "ft/getkey.h"
# include "ft/libft.h"
# include "ft/set.h"

# include "editor.h"

typedef struct s_editor_bind			t_editor_bind;
typedef struct s_editor_binding_set		t_editor_binding_set;

/*
** ========================================================================== **
*/

/*
** Base class for bindings
** -
** An editor_bind object can only be bind to a single editor_binding_set
*/
struct			s_editor_bind
{
	t_set_h			set_header;
	t_key			key;
	bool			(*f)(t_editor_bind *b, t_editor *editor);
	t_editor_bind	*next;
};

# define EDITOR_BIND(F)		((t_editor_bind){SET_HEAD(), KEY(0, 0), V(F), NULL})

/*
** Binding set
*/
struct			s_editor_binding_set
{
	t_set			bindings;
	uint32_t		extra_mods;
};

# define EDITOR_BINDING_SET()	((t_editor_binding_set){SET(&editor_bind_cmp, 0), 0})

/*
** Bind an editor_bind object to a key
*/
void			editor_bind(t_editor_binding_set *set, t_key key,
					t_editor_bind *b);

/*
** Execute bindings
** -
** If more than 1 editor_bind is bound to a key:
** 	the last bound is executed first
** 	the second is only executed if the first failed (return false)
** -
** Return false if no bindings are found or if they all fail
** Return true if a binding success
*/
bool			editor_key(t_editor_binding_set *set, t_editor *e, t_key key);

/*
** -
*/

int				editor_bind_cmp(t_editor_bind const *b, t_key const *key);

#endif
