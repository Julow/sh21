/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2015/12/11 18:35:37 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"
#include "ft/getkey.h"
#include "ft/term.h"

#include "editor.h"

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static bool		test_binding(t_editor *editor, t_key key)
{
	ft_dstradd(&editor->text, SUBC("looooooooool"));
	editor->cursor = 0;
	editor->sel = 0;
	// editor->sel = editor->text.length;
	return (true);
}

int				main(void)
{
	t_term *const	term = ft_tinit(1, TERM_RAW | TERM_LINE);
	// t_term *const	term = ft_tinit(1, TERM_RAW | TERM_FULLSCREEN);
	// t_term *const	term = ft_tinit(1, TERM_RAW);
	t_editor		editor;
	t_key			key;

	if (term->flags & TERM_USE_DEFAULT)
		ft_dprintf(2, "Warning: Invalid $TERM value: Use default: %s%n",
			TERM_DEFAULT_TERM);
	editor_init(&editor);
	editor_bind(&editor, KEY('C', KEY_MOD_SHIFT), &test_binding);
	ft_trestore(term, true);
	while (true)
	{
		key = ft_getkey(0);
		uint32_t line_count = term->line_count;
		ft_tclear(term);
		if (!editor_key(&editor, key))
			ft_fprintf(&term->out, "Unaccepted key: %3d '%c' Mods: %.4b (AC.S\\)%n", key.c,
				IS(key.c, IS_PRINT) ? key.c : 0, key.mods);
		ft_fprintf(&term->out, "Cursor: %d ; Sel: %d ; Len: %d ; "
			"Width: %d ; Lines: %d\n",
			editor.cursor, editor.sel, editor.text.length,
			term->width, line_count);
		editor_put(&editor, &term->out);
		ft_flush(&term->out);
		if (key.c == KEY_ESC && key.mods == 0)
			break ;
	}
	ft_trestore(term, false);
	ft_printf("exit%n");
	return (0);
}
