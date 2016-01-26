/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:27 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 15:03:07 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "editor_internal.h"

struct		s_put_pair
{
	t_editor const	*editor;
	t_out			*out;
};

static void	write_color(t_out *out, t_sub sub)
{
	if ((out->buff_size - out->buff_i) < (uint32_t)sub.length)
		ft_flush(out);
	ft_write(out, sub.str, sub.length);
}

static void	write_span(struct s_put_pair *p, t_vec2u span, t_style const *style)
{
	ft_printf("WRITE SPAN :: %u->%u :: %llX%n", span.x, span.y, style);
}

void		editor_put(t_editor const *editor, t_out *out)
{
	struct s_put_pair	p;
	int32_t				from;
	int32_t				to;

	p = (struct s_put_pair){editor, out};
	ft_putpad_left(out, editor->text.length);
	ft_spanlist_iter(&p, CALLBACK(write_span, editor),
		VEC2U(0, editor->text.length * 2));
	// if (editor->sel != 0)
	// {
	// 	from = editor->cursor;
	// 	to = editor->cursor;
	// 	if (editor->sel > 0)
	// 		to += editor->sel;
	// 	else
	// 		from += editor->sel;
	// 	ft_write(out, editor->text.str, from);
	// 	write_color(out, SUBC(BG_YELLOW));
	// 	ft_write(out, editor->text.str + from, to - from);
	// 	write_color(out, SUBC(BG_RESET));
	// 	ft_write(out, editor->text.str + to, editor->text.length - to);
	// }
	// else
	// 	ft_write(out, editor->text.str, editor->text.length);
	ft_putpad_right(out, editor->text.length);
}
