/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 21:38:38 by juloo             #+#    #+#             */
/*   Updated: 2016/02/04 16:42:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_BINDINGS_H
# define EDITOR_BINDINGS_H

# include "editor.h"

/*
** Action functions that can be passed to editor_bind()
*/

/*
** Put key:
** Write the char of the key if it is printable
*/
bool		editor_bind_putkey(void *data, t_editor *editor, t_key key);

/*
** Write string
*/
bool		editor_bind_write(char const *str, t_editor *editor);

/*
** Move cursor
** 'flags' is treated as an uintptr_t
** flags:
** 	CURSOR_MOVE_LEFT		Move to left (default is right)
** 	CURSOR_MOVE_SEL			Select char (default is to clear selection)
** modes: (flags but not cummulable)
** 	CURSOR_MOVE_BOUND		Move to file bound
** 	CURSOR_MOVE_LINEBOUND	Move to line bound
** 	CURSOR_MOVE_LINE		Move by line
** 	CURSOR_MOVE_WORD		Move by word (instead of by single char)
** 	CURSOR_MOVE_SUBWORD		Move by subword
*/
bool		editor_bind_cursor_move(void *flags, t_editor *editor);

# define CURSOR_MOVE_LEFT		(1 << 0)
# define CURSOR_MOVE_SEL		(1 << 1)
# define CURSOR_MOVE_BOUND		(1 << 2)
# define CURSOR_MOVE_LINEBOUND	(1 << 3)
# define CURSOR_MOVE_LINE		(1 << 4)
# define CURSOR_MOVE_WORD		(1 << 5)
# define _CURSOR_MOVE_SUBWORD	(1 << 6)
# define CURSOR_MOVE_SUBWORD	(CURSOR_MOVE_WORD | _CURSOR_MOVE_SUBWORD)

/*
** Delete char
** 'flags' is treated as an uintptr_t
** flags:
** 	DELETE_CLIPBOARD	Deleted chars are pushed on the clipboard
** 	CURSOR_MOVE_*		See cursor_move flags
*/
bool		editor_bind_delete(void *flags, t_editor *editor);

# define DELETE_CLIPBOARD		(1 << 7)

/*
** Paste from clipboard
** 'flags' is treated as an uintptr_t
** flags:
** 	PASTE_CONSUME		After pasting, pop from the clipboard
*/
bool		editor_bind_paste(void *flags, t_editor *editor);

# define PASTE_CONSUME			(1 << 0)

/*
** Add extra modifier
** 'flags' is treated as an uintptr_t
** 'flags' is added to modifier of the next key
** Can be used to create modifier
** Extra modifier are clear after each use
*/
bool		editor_bind_extra_mod(void *flags, t_editor *editor);

#endif
