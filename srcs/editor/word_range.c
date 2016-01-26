/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_range.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 15:39:38 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 19:39:38 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static const t_is	g_word_stops[] = {
	IS_CNTRL,
	IS_SPACE,
	IS_PUNCT,
	IS_WORD,
	0
};

static const t_is	g_subword_stops[] = {
	IS_CNTRL,
	IS_SPACE,
	IS_PUNCT,
	IS_UNDERSCORE,
	IS_DIGIT,
	IS_UPPER,
	IS_LOWER,
	0
};

t_vec2u			ft_word_range(t_sub sub, uint32_t at, bool subword)
{
	t_is const *const	stops = subword ? g_subword_stops : g_word_stops;
	t_vec2u				word;
	uint32_t			i;

	word = VEC2U(at, at);
	i = 0;
	while (stops[i] != 0)
	{
		while (word.x > 0 && IS(sub.str[word.x], stops[i]))
			word.x--;
		while (word.y < sub.length && IS(sub.str[word.y], stops[i]))
			word.y++;
		i++;
	}
	return (word);
}
