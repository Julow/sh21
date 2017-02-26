/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_default_bindings.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:37:24 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 19:41:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_DEFAULT_BINDINGS_H
# define EDITOR_DEFAULT_BINDINGS_H

# include "editor.h"
# include "editor_bindings.h"
# include "editor_utils.h"

typedef struct s_editor_move_bind		t_editor_move_bind;
typedef struct s_editor_put_bind		t_editor_put_bind;
typedef struct s_editor_delete_bind		t_editor_delete_bind;

/*
** ========================================================================== **
** Cursor move
** -
** move_f			=> Function that compute the new cursor position
** flags			=>
** 		EDITOR_MOVE_BACKWARD		Inverse direction
** 		EDITOR_MOVE_SEL				Select text while moving
** 		EDITOR_MOVE_ADD				Add new cursors instead of moving
*/

struct			s_editor_move_bind
{
	t_editor_bind	b;
	uint32_t		(*move_f)(t_editor *e, uint32_t pos, bool backward);
	uint32_t		flags;
};

#define EDITOR_MOVE_BACKWARD	(1 << 0)
#define EDITOR_MOVE_SEL			(1 << 1)
#define EDITOR_MOVE_ADD			(1 << 2)

#define EDITOR_MOVE_BIND(M,F)	((t_editor_move_bind){EDITOR_BIND(&editor_move_bind), (M), (F)})

bool			editor_move_bind(t_editor_move_bind *b, t_editor *e);

/*
** ========================================================================== **
** Put text
*/

struct			s_editor_put_bind
{
	t_editor_bind	b;
	t_sub			text;
};

#define EDITOR_PUT_BIND(T)	((t_editor_put_bind){EDITOR_BIND(&editor_put_bind), (T)})

bool			editor_put_bind(t_editor_put_bind *b, t_editor *e);

/*
** ========================================================================== **
** Delete text
** -
** move_f			=> Function that compute the end of the text to delete
** flags			=>
** 		EDITOR_DELETE_BACKWARD		Inverse direction
** 		EDITOR_DELETE_SEL			Try to delete only selection first
*/

struct			s_editor_delete_bind
{
	t_editor_bind	b;
	uint32_t		(*move_f)(t_editor *e, uint32_t pos, bool backward);
	uint32_t		flags;
};

#define EDITOR_DELETE_BACKWARD	(1 << 0)
#define EDITOR_DELETE_SEL		(1 << 1)

#define EDITOR_DELETE_BIND(M,F)	((t_editor_delete_bind){EDITOR_BIND(&editor_delete_bind), (M), (F)})

bool			editor_delete_bind(t_editor_delete_bind *b, t_editor *e);

#endif
