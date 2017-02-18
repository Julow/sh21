/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_default_bindings.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:37:24 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/18 14:44:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_DEFAULT_BINDINGS_H
# define EDITOR_DEFAULT_BINDINGS_H

# include "editor.h"
# include "editor_bindings.h"

typedef struct s_editor_move_bind		t_editor_move_bind;

/*
** ========================================================================== **
** Cursor move
** -
** move_f			=> Function that compute the new cursor position
** flags			=>
** 		EDITOR_MOVE_BACKWARD		Inverse direction
** 		EDITOR_MOVE_SEL				Select text while moving
*/

struct			s_editor_move_bind
{
	t_editor_bind	b;
	uint32_t		(*move_f)(t_editor *e, uint32_t pos, bool backward);
	uint32_t		flags;
};

#define EDITOR_MOVE_BACKWARD	(1 << 0)
#define EDITOR_MOVE_SEL			(1 << 1)

#define EDITOR_MOVE_BIND(T,F)	((t_editor_move_bind){EDITOR_BIND(&editor_move_bind), &editor_move_bind_##T, (F)})

/*
** -
*/

bool			editor_move_bind(t_editor_move_bind *b, t_editor *e);
uint32_t		editor_move_bind_line(t_editor *e, uint32_t pos, bool b);
uint32_t		editor_move_bind_linebound(t_editor *e, uint32_t pos, bool b);
uint32_t		editor_move_bind_bound(t_editor *e, uint32_t pos, bool b);
uint32_t		editor_move_bind_word(t_editor *e, uint32_t pos, bool b);
uint32_t		editor_move_bind_subword(t_editor *e, uint32_t pos, bool b);
uint32_t		editor_move_bind_char(t_editor *e, uint32_t pos, bool b);

#endif
