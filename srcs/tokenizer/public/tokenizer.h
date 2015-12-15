/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 17:19:33 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/15 18:21:49 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "ft/ft_base.h"

typedef struct s_token_def		t_token_def;

/*
** ========================================================================== **
** Tokenizer
*/

struct			s_token_def
{
	t_sub			sub;
	uint32_t		flags;
	void			*data;
};

/*
** The token_map is a map of t_sub * t_token_def
*/
bool			ft_tokenize(t_sub line, t_sub *token, void **data,
					t_hmap const *token_map);

#endif
