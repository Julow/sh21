/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 17:15:24 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/16 19:41:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/tokenizer.h"

bool			ft_tokenize(t_sub line, t_sub *token, t_token_t *type,
					t_token_map const *token_map)
{
	char const *const	end = line.str + line.length;
	t_sub				tmp;
	t_token_def			*t;

	token->str += token->length;
	token->length = 0;
	if (token->str >= end)
		return (false);
	if (type != NULL)
		*type = TOKEN_T_UNKNOWN;
	while (true)
	{
		tmp.str = token->str + token->length;
		if (tmp.str >= end)
			return (BOOL_OF(token->length > 0));
		if (BITARRAY_GET(token_map->token_starts, token->str[token->length]))
		{
			tmp.length = MIN(token_map->token_len, end - tmp.str - tmp.length);
			while (tmp.length > 0)
			{
				if ((t = ft_bst_get(&token_map->tokens, &tmp)) != NULL)
				{
					if (token->length > 0)
						return (true);
					token->length = t->sub.length;
					if (type != NULL)
						*type = t->type;
					return (true);
					break ;
				}
				tmp.length--;
			}
		}
		token->length++;
	}
}
