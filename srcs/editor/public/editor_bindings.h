/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 21:38:38 by juloo             #+#    #+#             */
/*   Updated: 2015/12/12 22:24:30 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_BINDINGS_H
# define EDITOR_BINDINGS_H

# include "editor.h"

/*
** Action functions that can be passed to editor_bind()
*/

bool			editor_bind_erase_sel(t_editor *editor, t_key key);
bool			editor_bind_backspace(t_editor *editor, t_key key);
bool			editor_bind_delete(t_editor *editor, t_key key);

bool			editor_bind_cursor_begin(t_editor *editor, t_key key);
bool			editor_bind_cursor_end(t_editor *editor, t_key key);

bool			editor_bind_cursor_left(t_editor *editor, t_key key);
bool			editor_bind_cursor_right(t_editor *editor, t_key key);

#endif
