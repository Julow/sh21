/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 17:19:33 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/16 19:42:04 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "ft/ft_bst.h"
# include "ft/libft.h"

typedef struct s_token_def		t_token_def;
typedef struct s_token_map		t_token_map;
typedef int32_t					t_token_t;

/*
** ========================================================================== **
** Tokenizer
*/

struct			s_token_def
{
	t_sub			sub;
	t_token_t		type;
};

struct			s_token_map
{
	t_bst			tokens;
	t_bitarray		token_starts[BITARRAY((uint8_t)-1)];
	int32_t			token_len;
};

/*
** Token that is not in the token_map
*/
# define TOKEN_T_UNKNOWN	(-1)

/*
** The token_map is a map of t_sub * t_token_def
** 'token' should be initialized before the first run with:
** 		SUB(line.str, 0)
** 'type' can be NULL
*/
bool			ft_tokenize(t_sub line, t_sub *token, t_token_t *type,
					t_token_map const *token_map);

/*
** Build a token map
*/
void			ft_token_map(t_token_map *map, t_token_def const *tokens,
					uint32_t count);

#endif
