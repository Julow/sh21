/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 17:15:24 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/17 00:41:56 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/tokenizer.h"

static bool		tokenize_from_map(t_sub match, t_sub *token, t_token_t *type,
					t_token_map const *token_map)
{
	t_token_def			*t;
	t_token_def			*max_t;

	t = NULL;
	max_t = NULL;
	while ((t = ft_bst_get(&token_map->tokens, t, &match)) != NULL)
		if (max_t == NULL || t->sub.length > max_t->sub.length)
			max_t = t;
	if (max_t == NULL)
		return (false);
	if (token->length > 0)
		return (true);
	token->length = max_t->sub.length;
	if (type != NULL)
		*type = max_t->type;
	return (true);
}

bool			ft_tokenize(t_sub line, t_sub *token, t_token_t *type,
					t_token_map const *token_map)
{
	char const *const	end = line.str + line.length;

	token->str += token->length;
	token->length = 0;
	if (type != NULL)
		*type = TOKEN_T_UNKNOWN;
	while (token->str + token->length < end)
	{
		if (BITARRAY_GET(token_map->token_starts, token->str[token->length])
			&& tokenize_from_map(SUB(token->str + token->length,
				end - token->str - token->length), token, type, token_map))
			return (true);
		token->length++;
	}
	return (BOOL_OF(token->length > 0));
}
