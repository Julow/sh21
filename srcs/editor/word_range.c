/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_range.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 15:39:38 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/14 16:40:30 by jaguillo         ###   ########.fr       */
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

t_range			ft_word_range(t_sub sub, int32_t at, bool subword)
{
	t_is const *const	stops = subword ? g_subword_stops : g_word_stops;
	t_range				word;
	uint32_t			i;

	word = RANGE(at, at);
	i = 0;
	while (stops[i] != 0)
	{
		while (word.from > 0 && IS(sub.str[word.from], stops[i]))
			word.from--;
		while (word.to < sub.length && IS(sub.str[word.to], stops[i]))
			word.to++;
		i++;
	}
	return (word);
}
