/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 17:19:33 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/19 01:21:37 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "ft/ft_bst.h"
# include "ft/libft.h"

typedef struct s_token_def		t_token_def;
typedef struct s_token_map		t_token_map;

/*
** ========================================================================== **
** Tokenizer
*/

struct			s_token_def
{
	t_sub			sub;
	void			*data;
};

struct			s_token_map
{
	t_bst			tokens;
	t_bitarray		token_starts[BITARRAY((uint8_t)-1)];
};

/*
** Init a token map
*/
# define TOKEN_MAP()	((t_token_map){BST(t_token_def, &token_map_cmp), {}})

/*
** The token_map is a map of t_sub * t_token_def
** 'token' should be initialized before the first run with:
** 		SUB(line.str, 0)
** 'data' can be NULL
** '*data' is set to the corresponding token's data
** 	or NULL if it's a unmatched token
*/
bool		ft_tokenize(t_sub line, t_sub *token, void **data,
					t_token_map const *token_map);

/*
** Add a token to a token_map
** token->sub data is copied
*/
void		ft_token_map(t_token_map *map, t_token_def const *token);

/*
** -
*/

int			token_map_cmp(t_token_def const *a, t_sub const *b);

#endif
