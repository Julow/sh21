/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:27 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/13 15:06:26 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "editor_internal.h"

static void	write_color(t_out *out, t_sub sub)
{
	if ((out->buff_size - out->buff_i) < (uint32_t)sub.length)
		ft_flush(out);
	ft_write(out, sub.str, sub.length);
}

void		editor_put(t_editor const *editor, t_out *out)
{
	ft_putpad_left(out, editor->text.length);
	if (editor->sel > 0)
	{
		ft_write(out, editor->text.str, editor->cursor);
		write_color(out, SUBC(BG_YELLOW));
		ft_write(out, editor->text.str + editor->cursor, editor->sel);
		write_color(out, SUBC(BG_RESET));
		ft_write(out, editor->text.str + editor->cursor + editor->sel,
			editor->text.length - editor->cursor - editor->sel);
	}
	else
		ft_write(out, editor->text.str, editor->text.length);
	ft_putpad_right(out, editor->text.length);
}
