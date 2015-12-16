/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/16 17:44:44 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "ft/ft_bst.h"
# include "ft/libft.h"
# include "ft/tokenizer.h"

typedef struct s_syntax			t_syntax;

typedef struct s_syntax_action	t_syntax_action;
typedef struct s_syntax_merge	t_syntax_merge;

/*
** ========================================================================== **
** Syntax
*/

struct			s_syntax_action
{
	t_sub			scope;
	t_sub			*syntax;
};

struct			s_syntax_merge
{
	t_token_t		a;
	t_token_t		b;
	t_token_t		dst;
};

struct			s_syntax
{
	t_token_map		tokens;
	t_token_t		end;
	// token_unmatch<t_is, t_token_t>
	t_bst			token_merge;
	t_bst			actions;
};

#endif
