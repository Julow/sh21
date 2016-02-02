/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:35:25 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 00:11:32 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

// // implicit:
// // 	bind backspace			delete left
// // 	bind left				cursor_move left
// // 	bind right				cursor_move

// bind delete				delete right
// bind ctrl+backspace		delete left word
// bind ctrl+delete			delete right word
// bind alt+backspace		delete left subword
// bind alt+delete			delete right subword

// TODO bind ctrl+c				delete line

// bind ctrl+w				delete left word clipboard
// bind ctrl+k				delete right bound clipboard
// bind ctrl+o				delete left bound clipboard

// bind ctrl+y				paste
// bind ctrl+v				paste consume

// bind home				cursor_move left bound
// bind end					cursor_move right bound
// bind ctrl+a				cursor_move left bound
// bind ctrl+e				cursor_move right bound
// bind left				cursor_move left
// bind right				cursor_move right
// bind shift+left			cursor_move sel left
// bind shift+right			cursor_move sel right
// bind ctrl+left			cursor_move left word
// bind ctrl+right			cursor_move right word
// bind ctrl+shift+left		cursor_move sel left word
// bind ctrl+shift+right	cursor_move sel right word
// bind alt+left			cursor_move left subword
// bind alt+right			cursor_move right subword
// bind alt+shift+left		cursor_move sel left subword
// bind alt+shift+right		cursor_move sel right subword

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

	{KEY('w', KEY_MOD_CTRL), &editor_bind_delete, DELETE_LEFT | DELETE_WORD | DELETE_CLIPBOARD},
	{KEY('k', KEY_MOD_CTRL), &editor_bind_delete, DELETE_BOUND | DELETE_CLIPBOARD},
	{KEY('o', KEY_MOD_CTRL), &editor_bind_delete, DELETE_LEFT | DELETE_BOUND | DELETE_CLIPBOARD},

	{KEY('y', KEY_MOD_CTRL), &editor_bind_paste, 0},
	{KEY('v', KEY_MOD_CTRL), &editor_bind_paste, PASTE_CONSUME},

	{KEY('s', KEY_MOD_CTRL), &editor_bind_extra_mod, KEY_MOD_SHIFT},

	{KEY(KEY_LEFT, 0), &editor_bind_cursor_move, CURSOR_MOVE_LEFT},
	{KEY(KEY_RIGHT, 0), &editor_bind_cursor_move, 0},
	{KEY(KEY_LEFT, KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL},
	{KEY(KEY_RIGHT, KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL},
	{KEY(KEY_LEFT, KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_WORD},
	{KEY(KEY_RIGHT, KEY_MOD_CTRL), &editor_bind_cursor_move, CURSOR_MOVE_WORD},
	{KEY(KEY_LEFT, KEY_MOD_CTRL | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_WORD},
	{KEY(KEY_RIGHT, KEY_MOD_CTRL | KEY_MOD_SHIFT), &editor_bind_cursor_move, CURSOR_MOVE_SEL | CURSOR_MOVE_WORD},
	{KEY(KEY_LEFT, KEY_MOD_ALT), &editor_bind_cursor_move, CURSOR_MOVE_LEFT | CURSOR_MOVE_SUBWORD},
	{KEY(KEY_RIGHT, KEY_MOD_ALT), &editor_bind_cursor_move, CURSOR_MOVE_SUBWORD},
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
		VECTOR(uint32_t),
		SPANLIST(t_style),
		0,
		0,
		BST(t_binding, &editor_binding_cmp),
		LIST(t_sub),
		0,
		NULL
	};
	i = 0;
	ft_vpush(&editor->line_stops, &i, 1);
	while (i < ARRAY_LEN(g_bindings))
	{
		editor_bind(editor, g_bindings[i].key, g_bindings[i].f,
			g_bindings[i].flags);
		i++;
	}
}
