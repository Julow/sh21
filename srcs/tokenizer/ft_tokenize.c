/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 17:15:24 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/17 18:09:26 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/tokenizer.h"

static bool		find_max_t(t_token_def const *def, t_sub const *match,
					t_token_def const **token)
{
	if ((*token == NULL || (*token)->sub.length < def->sub.length)
		&& ft_memcmp(match->str, def->sub.str, def->sub.length) == 0)
		*token = def;
	return (true);
}

static bool		tokenize_from_map(t_sub match, t_sub *token, t_token_t *type,
					t_token_map const *token_map)
{
	t_token_def			*t;

	t = NULL;
	ft_bst_getall(&token_map->tokens, &match, &find_max_t, &t);
	if (t == NULL)
		return (false);
	if (token->length > 0)
		return (true);
	token->length = t->sub.length;
	if (type != NULL)
		*type = t->type;
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
