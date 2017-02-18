/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_move_bind_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:46:50 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/18 14:51:32 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_default_bindings.h"

static uint32_t const	g_word_stops[][8] = {
	{
		IS_WORD,
		0
	},
	{
		IS_CNTRL,
		IS_SPACE,
		IS_PUNCT,
		IS_WORD,
		0
	},
	{
		IS_CNTRL,
		IS_SPACE,
		IS_PUNCT,
		IS_UNDERSCORE,
		IS_DIGIT,
		IS_UPPER,
		IS_LOWER,
		0
	},
};

/*
** TODO: move
*/
static t_vec2u	editor_word_range(t_editor *e, uint32_t pos, uint32_t level)
{
	uint32_t const *const	stops = g_word_stops[level];
	t_vec2u					word;
	uint32_t				i;

	word = VEC2U(pos, pos);
	i = 0;
	while (stops[i] != 0)
	{
		while (word.x > 0 && IS(e->text.str[word.x], stops[i]))
			word.x--;
		while (word.y < e->text.length && IS(e->text.str[word.y], stops[i]))
			word.y++;
		i++;
	}
	return (word);
}

uint32_t		editor_move_bind_word(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	word_bounds = editor_word_range(e, pos, 1);

	return (b ? word_bounds.x : word_bounds.y);
}

uint32_t		editor_move_bind_subword(t_editor *e, uint32_t pos, bool b)
{
	t_vec2u const	word_bounds = editor_word_range(e, pos, 2);

	return (b ? word_bounds.x : word_bounds.y);
}
