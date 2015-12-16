/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:06:57 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/17 00:42:21 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/tokenizer.h"

static int		token_cmp(t_token_def const *a, t_sub const *b)
{
	return (ft_memcmp(b->str, a->sub.str, MIN(a->sub.length, b->length)));
}

void			ft_token_map(t_token_map *map, t_token_def const *tokens,
					uint32_t count)
{
	uint32_t		i;
	t_token_def		*t;

	*map = (t_token_map){
		BST(t_token_def, &token_cmp),
		{},
		0
	};
	i = 0;
	while (i < count)
	{
		ASSERT(tokens[i].sub.length > 0);
		if (tokens[i].sub.length > map->token_len)
			map->token_len = tokens[i].sub.length;
		BITARRAY_SET(map->token_starts, tokens[i].sub.str[0]);
		t = ft_bst_put(&map->tokens, &tokens[i].sub, tokens[i].sub.length);
		*t = (t_token_def){
			SUB(ENDOF(t), tokens[i].sub.length),
			tokens[i].type
		};
		ft_memcpy(ENDOF(t), tokens[i].sub.str, tokens[i].sub.length);
		i++;
	}
}
