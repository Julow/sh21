/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:37:52 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/20 22:06:12 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_term.h"

// void			editor_term_update_line(t_editor_term *t, uint32_t line,
// 					t_vec2u range)
// {
// 	uint32_t		index;
// 	uint32_t		line_end;

// 	if (line < t->scroll.y || line >= t->scroll.y + t->size.y)
// 		return ;
// 	range = VEC2U(MAX(range.x, t->scroll.x),
// 			MIN(range.y, t->scroll.x + t->size.x));
// 	if (range.x >= range.y)
// 		return ;
// 	ft_tcursor(t->term, range.x - scroll.x, line - scroll.y);
// 	index = editor_getindex(t->editor, VEC2U(line, range.x));
// 	line_end = MAX(EDITOR_LINE(editor, line), scroll.x) - scroll.x;
// 	if (line_end > (range.y - range.x))
// 	{
// 		ft_write(V(t->term), t->editor->text.str + index, range.y - range.x);
// 	}
// 	else
// 	{
// 		ft_write(V(t->term), t->editor->text.str + index, line_end);
// 		ft_write_nchar(V(t->term), ' ', range.y - range.x - line_end);
// 		// TODO: tput ce
// 	}
// }

void			editor_term_update_redraw(t_editor_term *t,
					t_editor_term_update const *update)
{
	(void)t;
	(void)update;
}

static void		put_line(t_editor_term *t, uint32_t line)
{
	uint32_t		index;
	uint32_t		line_end;

	index = editor_getindex(t->editor, VEC2U(line, t->scroll.x));
	line_end = MAX(EDITOR_LINE(t->editor, line), t->scroll.x) - t->scroll.x;
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
	ft_tcursor(t->term, 0, t->size.y - (range.y - range.x));
	// TODO: ft_tput_dl(t->term, range.y - range.x);
	ft_tcursor(t->term, 0, range.x - t->scroll.y + 1);
	// TODO: ft_tput_al(t->term, range.y - range.x);
	while (range.x < range.y)
		put_line(t, range.x++);

}

// void			editor_term_remove_line(t_editor_term *t, uint32_t at,
// 					uint32_t count)
// {
// 	// if (at < scroll.y)
// 	// {
// 	// 	scroll.y--;
// 	// 	return ;
// 	// }
// 	// if (at >= scroll.y + size.y)
// 	// 	return ;
// 	// ft_tcursor(t->term, 0, at - scroll.y);
// 	// tput dl
// 	// ft_tcursor(t->term, 0, size.y - 1);
// 	// put_line(t, scroll.y + size.y - 1);

// 	uint32_t		end;

// 	end = MIN(line + count, scroll.y + size.y);
// 	if (line < scroll.y)
// 	{
// 		line = scroll.y;
// 		scroll.y -= MIN(count, scroll.y - line);
// 	}
// 	if (line >= end)
// 		return ;
// 	count = end - line;
// }

void			editor_term_update_delete_line(t_editor_term *t,
					t_editor_term_update const *update)
{
	(void)t;
	(void)update;
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
	t->updates.length = 0;
}
