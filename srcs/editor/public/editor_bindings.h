/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 21:38:38 by juloo             #+#    #+#             */
/*   Updated: 2015/12/14 18:27:02 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_BINDINGS_H
# define EDITOR_BINDINGS_H

# include "editor.h"

/*
** Action functions that can be passed to editor_bind()
*/

# define CURSOR_MOVE_LEFT		(1 << 0)
# define CURSOR_MOVE_SEL		(1 << 1)
# define CURSOR_MOVE_BOUND		(1 << 2)
# define CURSOR_MOVE_WORD		(1 << 3)
# define _CURSOR_MOVE_SUBWORD	(1 << 4)
# define CURSOR_MOVE_SUBWORD	(CURSOR_MOVE_WORD | _CURSOR_MOVE_SUBWORD)

bool		editor_bind_cursor_move(t_editor *editor, uint32_t flags);

# define DELETE_LEFT			CURSOR_MOVE_LEFT
# define DELETE_BOUND			CURSOR_MOVE_BOUND
# define DELETE_WORD			CURSOR_MOVE_WORD
# define DELETE_SUBWORD			CURSOR_MOVE_SUBWORD

bool		editor_bind_delete(t_editor *editor, uint32_t flags);

#endif
