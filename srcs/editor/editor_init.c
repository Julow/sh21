/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:35:25 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/11 18:07:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static bool	editor_binding_del(t_editor *editor, t_key key) // TODO: move
{
	if (editor->sel > 0)
	{
		ft_dstrspan(&editor->text, editor->cursor,
			editor->cursor + editor->sel, 0);
		editor->sel = 0;
	}
	else if (key.c == KEY_BACKSPACE && editor->cursor > 0)
	{
		editor->cursor--;
		editor->text.length--;
	}
	else if (key.c == KEY_DELETE && editor->cursor < (uint32_t)editor->text.length)
		ft_dstrspan(&editor->text, editor->cursor, editor->cursor + 1, 0);
	else
		return (false);
	return (true);
}

static t_binding const	g_bindings[] = {
	{KEY(KEY_BACKSPACE, 0), &editor_binding_del},
	{KEY(KEY_DELETE, 0), &editor_binding_del},
	{KEY('m', KEY_MOD_CTRL), &editor_binding_nl},
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
