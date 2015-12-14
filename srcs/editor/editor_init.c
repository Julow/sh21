/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:35:25 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/14 18:27:33 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

static t_binding const	g_bindings[] = {
	{KEY(KEY_BACKSPACE, 0), &editor_bind_delete, DELETE_LEFT},
	{KEY(KEY_DELETE, 0), &editor_bind_delete, 0},
	{KEY(KEY_BACKSPACE, KEY_MOD_CTRL), &editor_bind_delete, DELETE_WORD | DELETE_LEFT},
	{KEY(KEY_DELETE, KEY_MOD_CTRL), &editor_bind_delete, DELETE_WORD},
	{KEY(KEY_BACKSPACE, KEY_MOD_ALT), &editor_bind_delete, DELETE_SUBWORD | DELETE_LEFT},
	{KEY(KEY_DELETE, KEY_MOD_ALT), &editor_bind_delete, DELETE_SUBWORD},

	{KEY(KEY_HOME, 0), &editor_bind_cursor_move, CURSOR_MOVE_BOUND | CURSOR_MOVE_LEFT},
	{KEY(KEY_END, 0), &editor_bind_cursor_move, CURSOR_MOVE_BOUND},
	{KEY('a', KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_BOUND | CURSOR_MOVE_LEFT},
	{KEY('e', KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_BOUND},

	{KEY(KEY_LEFT, 0), &editor_bind_cursor_move, CURSOR_MOVE_LEFT},
	{KEY(KEY_RIGHT, 0), &editor_bind_cursor_move, 0},
	{KEY(KEY_LEFT, KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL},
	{KEY(KEY_RIGHT, KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL},
	{KEY(KEY_LEFT, KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_WORD},
	{KEY(KEY_RIGHT, KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_WORD},
	{KEY(KEY_LEFT, KEY_MOD_CTRL | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_WORD},
	{KEY(KEY_RIGHT, KEY_MOD_CTRL | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL | CURSOR_MOVE_WORD},
	{KEY(KEY_LEFT, KEY_MOD_ALT | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD},
	{KEY(KEY_RIGHT, KEY_MOD_ALT | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD},
	{KEY(KEY_LEFT, KEY_MOD_ALT | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD},
	{KEY(KEY_RIGHT, KEY_MOD_ALT | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD},
};

static int	editor_binding_cmp(t_binding const *a, t_key const *b)
{
	if (a->key.c == b->c)
		return (a->key.mods - b->mods);
	return (a->key.c - b->c);
}

void		editor_init(t_editor *editor)
{
	uint32_t	i;

	*editor = (t_editor){
		DSTR0(),
		0,
		0,
		BST(t_binding, &editor_binding_cmp)
	};
	i = 0;
	while (i < ARRAY_LEN(g_bindings))
	{
		editor_bind(editor, g_bindings[i].key, g_bindings[i].f,
			g_bindings[i].flags);
		i++;
	}
}
