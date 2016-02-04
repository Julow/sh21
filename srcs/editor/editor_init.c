/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:35:25 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:48:05 by jaguillo         ###   ########.fr       */
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

#define BIND(K,MOD,F,DATA,P)	{KEY(K,MOD),CALLBACK(F,DATA),(P),NULL}

static t_binding const	g_bindings[] = {

	BIND(-1, 0,								editor_bind_putkey,			NULL,															0),
	BIND(-1, MOD_SHIFT,						editor_bind_putkey,			NULL,															0),

	BIND(KEY_BACKSPACE, 0,					editor_bind_delete,			V(DELETE_LEFT),													0),
	BIND(KEY_DELETE, 0,						editor_bind_delete,			V(0),															0),
	BIND(KEY_BACKSPACE, MOD_CTRL,			editor_bind_delete,			V(DELETE_WORD | DELETE_LEFT),									0),
	BIND(KEY_DELETE, MOD_CTRL,				editor_bind_delete,			V(DELETE_WORD),													0),
	BIND(KEY_BACKSPACE, MOD_ALT,			editor_bind_delete,			V(DELETE_SUBWORD | DELETE_LEFT),								0),
	BIND(KEY_DELETE, MOD_ALT,				editor_bind_delete,			V(DELETE_SUBWORD),												0),

	BIND(KEY_HOME, 0,						editor_bind_cursor_move,	V(CURSOR_MOVE_BOUND | CURSOR_MOVE_LEFT),						0),
	BIND(KEY_END, 0,						editor_bind_cursor_move,	V(CURSOR_MOVE_BOUND),											0),
	BIND('a', MOD_CTRL,						editor_bind_cursor_move,	V(CURSOR_MOVE_BOUND | CURSOR_MOVE_LEFT),						0),
	BIND('e', MOD_CTRL,						editor_bind_cursor_move,	V(CURSOR_MOVE_BOUND),											0),

	BIND('w', MOD_CTRL,						editor_bind_delete,			V(DELETE_LEFT | DELETE_WORD | DELETE_CLIPBOARD),				0),
	BIND('k', MOD_CTRL,						editor_bind_delete,			V(DELETE_BOUND | DELETE_CLIPBOARD),								0),
	BIND('o', MOD_CTRL,						editor_bind_delete,			V(DELETE_LEFT | DELETE_BOUND | DELETE_CLIPBOARD),				0),

	BIND('y', MOD_CTRL,						editor_bind_paste,			V(0),															0),
	BIND('v', MOD_CTRL,						editor_bind_paste,			V(PASTE_CONSUME),												0),

	BIND('s', MOD_CTRL,						editor_bind_extra_mod,		V(MOD_SHIFT),												0),

	BIND(KEY_LEFT, 0,						editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT),											0),
	BIND(KEY_RIGHT, 0,						editor_bind_cursor_move,	V(0),															0),
	BIND(KEY_LEFT, MOD_SHIFT,				editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL),							0),
	BIND(KEY_RIGHT, MOD_SHIFT,				editor_bind_cursor_move,	V(CURSOR_MOVE_SEL),												0),
	BIND(KEY_LEFT, MOD_CTRL,				editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT | CURSOR_MOVE_WORD),							0),
	BIND(KEY_RIGHT, MOD_CTRL,				editor_bind_cursor_move,	V(CURSOR_MOVE_WORD),											0),
	BIND(KEY_LEFT, MOD_CTRL | MOD_SHIFT,	editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_WORD),		0),
	BIND(KEY_RIGHT, MOD_CTRL | MOD_SHIFT,	editor_bind_cursor_move,	V(CURSOR_MOVE_SEL | CURSOR_MOVE_WORD),							0),
	BIND(KEY_LEFT, MOD_ALT,					editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT | CURSOR_MOVE_SUBWORD),						0),
	BIND(KEY_RIGHT, MOD_ALT,				editor_bind_cursor_move,	V(CURSOR_MOVE_SUBWORD),											0),
	BIND(KEY_LEFT, MOD_ALT | MOD_SHIFT,		editor_bind_cursor_move,	V(CURSOR_MOVE_LEFT | CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD),	0),
	BIND(KEY_RIGHT, MOD_ALT | MOD_SHIFT,	editor_bind_cursor_move,	V(CURSOR_MOVE_SEL | CURSOR_MOVE_SUBWORD),						0),

};

static int	editor_binding_cmp(t_binding const *const *a, t_key const *b)
{
	if ((*a)->key.c == b->c)
		return ((*a)->key.mods - b->mods);
	return ((*a)->key.c - b->c);
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
		BST(t_binding*, &editor_binding_cmp),
		LIST(t_sub),
		0,
		NULL
	};
	i = 1; // WTF ? TODO: Fix editor_write
	ft_vpush(&editor->line_stops, &i, 1);
	i = 0;
	while (i < ARRAY_LEN(g_bindings))
	{
		editor_bind(editor, g_bindings[i].key, g_bindings[i].callback,
			g_bindings[i].priority);
		i++;
	}
}
