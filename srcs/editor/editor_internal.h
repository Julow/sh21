/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:36:38 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 11:46:48 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_INTERNAL_H
# define EDITOR_INTERNAL_H

# include "editor.h"

typedef struct s_binding		t_binding;

struct 		s_binding
{
	t_key		key;
	t_callback	callback;
	int32_t		priority;
	t_binding	*next;
};

/*
** Return the offset to move respecting
** CURSOR_MOVE_LEFT, CURSOR_MOVE_WORD and CURSOR_MOVE_SUBWORD
*/
int32_t			cursor_move(t_editor *editor, uint32_t flags);

#endif
