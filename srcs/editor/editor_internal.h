/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:36:38 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/14 18:27:57 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_INTERNAL_H
# define EDITOR_INTERNAL_H

# include "editor.h"

typedef struct s_binding		t_binding;

struct 		s_binding
{
	t_key		key;
	bool		(*f)(t_editor*, uint32_t);
	uint32_t	flags;
};

/*
** Return the offset to move respecting
** CURSOR_MOVE_LEFT, CURSOR_MOVE_WORD and CURSOR_MOVE_SUBWORD
*/
int32_t			cursor_move(t_editor *editor, uint32_t flags);

#endif
