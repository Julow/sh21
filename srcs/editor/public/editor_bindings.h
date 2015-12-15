/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 21:38:38 by juloo             #+#    #+#             */
/*   Updated: 2015/12/15 19:09:35 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_BINDINGS_H
# define EDITOR_BINDINGS_H

# include "editor.h"

/*
** Action functions that can be passed to editor_bind()
*/

/*
** Move cursor
** flags:
** 	CURSOR_MOVE_LEFT	Move to left (default is right)
** 	CURSOR_MOVE_SEL		Select char (default is to clear selection)
** 	CURSOR_MOVE_BOUND	Go to end of line (or start with CURSOR_MOVE_LEFT)
** 	CURSOR_MOVE_WORD	Move by word (instead of by single char)
** 	CURSOR_MOVE_SUBWORD	Move by subword
*/
bool		editor_bind_cursor_move(t_editor *editor, uint32_t flags);

# define CURSOR_MOVE_LEFT		(1 << 0)
# define CURSOR_MOVE_SEL		(1 << 1)
# define CURSOR_MOVE_BOUND		(1 << 2)
# define CURSOR_MOVE_WORD		(1 << 3)
# define _CURSOR_MOVE_SUBWORD	(1 << 4)
# define CURSOR_MOVE_SUBWORD	(CURSOR_MOVE_WORD | _CURSOR_MOVE_SUBWORD)

/*
** Delete char
** flags:
** 	DELETE_LEFT			Like CURSOR_MOVE_LEFT
** 	DELETE_BOUND		Like CURSOR_MOVE_BOUND
** 	DELETE_WORD			Like CURSOR_MOVE_WORD
** 	DELETE_SUBWORD		Like CURSOR_MOVE_SUBWORD
** 	DELETE_CLIPBOARD	Deleted chars are pushed on the clipboard
*/
bool		editor_bind_delete(t_editor *editor, uint32_t flags);

# define DELETE_LEFT			CURSOR_MOVE_LEFT
# define DELETE_BOUND			CURSOR_MOVE_BOUND
# define DELETE_WORD			CURSOR_MOVE_WORD
# define DELETE_SUBWORD			CURSOR_MOVE_SUBWORD
# define DELETE_CLIPBOARD		(1 << 5)

/*
** Paste from clipboard
** flags:
** 	PASTE_CONSUME		After pasting, pop from the clipboard
*/
bool		editor_bind_paste(t_editor *editor, uint32_t flags);

# define PASTE_CONSUME			(1 << 0)

/*
** Add extra modifier
** 'flags' is added to modifier of the next key
** Can be used to create modifier
** Extra modifier are clear after each use
*/
bool		editor_bind_extra_mod(t_editor *editor, uint32_t flags);

#endif
