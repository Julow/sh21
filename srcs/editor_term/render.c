/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:37:52 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/23 20:12:36 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h"

#include "editor_term.h"
#include "p_editor_term.h"

/*
** Return the indexes of the first/last cursors that collide 'range'
*/
static t_vec2u	get_cursors(t_editor const *editor, t_vec2u range)
{
	t_vec2u				i;
	t_editor_sel const	*sel;

	i.x = 0;
	while (i.x < editor->cursors.length)
	{
		sel = VECTOR_GET(editor->cursors, i.x);
		if (EDITOR_SEL_END(*sel) > range.x)
			break ;
		i.x++;
	}
	i.y = i.x;
	while (i.y < editor->cursors.length)
	{
		sel = VECTOR_GET(editor->cursors, i.y);
		if (EDITOR_SEL_BEGIN(*sel) >= range.y)
			break ;
		i.y++;
	}
	return (i);
}

static void		redraw_text(t_editor_term *t, uint32_t index, uint32_t length)
{
	t_vec2u				cursors;
	t_vec2u				range;
	t_editor_sel 		sel;

	range = VEC2U(index, index + length);
	cursors = get_cursors(t->editor, range);
	while (cursors.x < cursors.y)
	{
		sel = VGETC(t_editor_sel, t->editor->cursors, cursors.x);
		if (EDITOR_SEL_BEGIN(sel) > range.x)
		{
			ft_write(V(t->term), t->editor->text.str + range.x,
					range.x - EDITOR_SEL_BEGIN(sel));
			range.x = EDITOR_SEL_BEGIN(sel);
		}
		if (sel.x > sel.y)
		{
			range.x = MIN(range.y, range.x + sel.x - 1 - sel.y);
			ASSERT(range.x >= sel.x, "MAX(range.x, sel.y) - sel.y");
			ft_fprintf(V(t->term), "\033[46m%ts\033[49m",
					SUB(t->editor->text.str + sel.y, range.x - sel.y));
		}
		if (cursors.x > 0 && range.x < range.y)
			ft_fprintf(V(t->term), "\033[107m%c\033[49m",
					t->editor->text.str[range.x++]);
		if (sel.x < sel.y)
		{
			range.x = MIN(range.y, range.x + sel.y - sel.x - 1);
			ASSERT(range.x >= sel.x, "MAX(range.x, sel.x) - sel.x");
			ft_fprintf(V(t->term), "\033[46m%ts\033[49m",
					SUB(t->editor->text.str + sel.x, range.x - sel.x));
		}
		cursors.x++;
	}
	ft_write(V(t->term), t->editor->text.str + range.x, range.y - range.x);
}

void			editor_term_update_redraw(t_editor_term *t,
					t_editor_term_update const *update)
{
	t_vec3u			area;
	uint32_t		line_len;

	area = VEC3U(update->redraw.x, MAX(update->redraw.y, t->scroll.x),
			MIN(update->redraw.y + update->redraw.z, t->scroll.x + t->size.x));
	if (area.x < t->scroll.y || area.x >= t->scroll.y + t->size.y
			|| area.y >= area.z)
		return ;
	ft_tcursor(t->term, area.y - t->scroll.x, area.x - t->scroll.y);
	line_len = EDITOR_LINE(t->editor, area.x);
	redraw_text(t, editor_getindex(t->editor, VEC2U_3(area)),
			MAX(MIN(line_len, area.z), area.y) - area.y);
	if (line_len < area.z)
		ft_tput(t->term, TERM_CAP_CE);
}

static void		put_line(t_editor_term *t, uint32_t line)
{
	uint32_t		index;
	uint32_t		line_end;

	index = editor_getindex(t->editor, VEC2U(line, t->scroll.x));
	line_end = MAX(EDITOR_LINE(t->editor, line), 1) - 1;
	line_end = MAX(line_end, t->scroll.x) - t->scroll.x;
	ft_write(V(t->term), t->editor->text.str + index, MIN(line_end, t->size.x));
}

void			editor_term_update_insert_line(t_editor_term *t,
					t_editor_term_update const *update)
{
	t_vec2u			range;

	range = VEC2U(update->redraw.x,
			MIN(update->redraw.x + update->redraw.y, t->scroll.y + t->size.y));
	if (range.x < t->scroll.y)
	{
		t->scroll.y += MIN(update->redraw.y, t->scroll.y - range.x);
		range.x = t->scroll.y;
	}
	if (range.x >= range.y)
		return ;
	ft_tcursor(t->term, 0, range.x - t->scroll.y);
	ft_tline(t->term, range.y - range.x);
	while (range.x < range.y)
	{
		put_line(t, range.x++);
	}
}

void			editor_term_update_delete_line(t_editor_term *t,
					t_editor_term_update const *update)
{
	t_vec2u			range;

	range = VEC2U(update->redraw.x,
			MIN(update->redraw.x + update->redraw.y, t->scroll.y + t->size.y));
	if (range.x < t->scroll.y)
	{
		t->scroll.y -= MIN(update->redraw.y, t->scroll.y - range.x);
		range.x = t->scroll.y;
	}
	if (range.x >= range.y)
		return ;
	ft_tcursor(t->term, 0, range.x - t->scroll.y);
	ft_tline(t->term, -(range.y - range.x));
	ft_tcursor(t->term, 0, t->size.y - (range.y - range.x));
	while (range.x < range.y)
	{
		put_line(t, t->size.y - (range.y - range.x));
		ft_write_char(V(t->term), '\n');
		range.x++;
	}
}

void			editor_term_update_scroll(t_editor_term *t,
					t_editor_term_update const *update)
{
	(void)t;
	(void)update;
}

static void			(*const g_editor_term_update[])(t_editor_term *t,
						t_editor_term_update const *update) = {
	[EDITOR_TERM_UPDATE_REDRAW] = &editor_term_update_redraw,
	[EDITOR_TERM_UPDATE_INSERT_LINE] = &editor_term_update_insert_line,
	[EDITOR_TERM_UPDATE_DELETE_LINE] = &editor_term_update_delete_line,
	[EDITOR_TERM_UPDATE_SCROLL] = &editor_term_update_scroll,
};

void			editor_render(t_editor_term *t)
{
	t_editor_term_update const	*update;

	update = VECTOR_IT(t->updates);
	while (VECTOR_NEXT(t->updates, update))
		g_editor_term_update[update->type](t, update);
	ft_flush(V(t->term));
	t->updates.length = 0;
}

void			editor_rerender(t_editor_term *t)
{
	uint32_t		i;
	uint32_t const	end = MIN(t->size.y, t->editor->lines.length - t->scroll.y);

	i = 0;
	while (i < end)
	{
		ft_tcursor(t->term, 0, i);
		put_line(t, i + t->scroll.y);
		i++;
	}
	ft_flush(V(t->term));
	t->updates.length = 0;
}
