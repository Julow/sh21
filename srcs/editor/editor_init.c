/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:35:25 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/13 13:45:29 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"
#include "editor_internal.h"

static bool	editor_binding_nl(t_editor *editor, t_key key)
{
	*ft_dstrspan(&editor->text, editor->cursor,
		editor->cursor + editor->sel, 1) = '\n';
	editor->cursor++;
	editor->sel = 0;
	(void)key;
	return (true);
}

static t_binding const	g_bindings[] = {
	{KEY(KEY_BACKSPACE, 0), &editor_bind_backspace},
	{KEY(KEY_DELETE, 0), &editor_bind_delete},
	{KEY('m', KEY_MOD_CTRL), &editor_binding_nl},

	{KEY(KEY_HOME, 0), &editor_bind_cursor_begin},
	{KEY(KEY_END, 0), &editor_bind_cursor_end},
	{KEY('a', KEY_MOD_CTRL), &editor_bind_cursor_begin},
	{KEY('e', KEY_MOD_CTRL), &editor_bind_cursor_end},

	{KEY(KEY_LEFT, 0), &editor_bind_cursor_left},
	{KEY(KEY_RIGHT, 0), &editor_bind_cursor_right},
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
		editor_bind(editor, g_bindings[i].key, g_bindings[i].f);
		i++;
	}
}
