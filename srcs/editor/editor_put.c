/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:34:27 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 18:43:03 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "editor_internal.h"

struct		s_put_pair
{
	t_editor const	*editor;
	t_out			*out;
};

#define WRITE_C(OUT,STR)	ft_write(OUT, STR, sizeof(STR))

static void	put_color(t_out *out, uint32_t color, bool background)
{
	uint32_t	c;

	if ((out->buff_i + ((color & STYLE_F_EXTENDED) ? 10 : 5)) >= out->buff_size)
		ft_flush(out);
	c = color & ~(STYLE_F_EXTENDED | STYLE_F_LIGHT);
	if (!(color & STYLE_F_EXTENDED))
	{
		if (color & STYLE_F_LIGHT)
			c += background ? 100 : 90;
		else
			c += background ? 40 : 30;
	}
	ft_fprintf(out, ((color & STYLE_F_EXTENDED) ? "\033[48;5;%um" : "\033[%um"),
		c);
}

static void	write_span(struct s_put_pair *p, t_vec2u span, t_style const *style)
{
	if (style != NULL)
	{
		if (style->background != 0)
			put_color(p->out, style->background, true);
		if (style->foreground != 0)
			put_color(p->out, style->foreground, false);
		if (style->styles & S_UNDERLINE)
			ft_putsub(p->out, SUBC("\033[4m"));
		if (style->styles & S_BOLD)
			ft_putsub(p->out, SUBC("\033[1m"));
		if (style->styles & S_REVERSED)
			ft_putsub(p->out, SUBC("\033[7m"));
	}
	ft_putsub(p->out, SUB(p->editor->text.str + span.x, span.y - span.x));
	if (style != NULL)
	{
		if (style->foreground != 0)
			ft_putsub(p->out, SUBC("\033[39m"));
		if (style->background != 0)
			ft_putsub(p->out, SUBC("\033[49m"));
		if (style->styles & S_UNDERLINE)
			ft_putsub(p->out, SUBC("\033[24m"));
		if (style->styles & S_BOLD)
			ft_putsub(p->out, SUBC("\033[22m"));
		if (style->styles & S_REVERSED)
			ft_putsub(p->out, SUBC("\033[27m"));
	}
}

void		editor_put(t_editor const *editor, t_out *out)
{
	struct s_put_pair	p;

	p = (struct s_put_pair){editor, out};
	ft_putpad_left(out, editor->text.length);
	ft_spanlist_iter(&editor->spans, CALLBACK(write_span, &p),
		VEC2U(0, editor->text.length));
	ft_putpad_right(out, editor->text.length);
}
