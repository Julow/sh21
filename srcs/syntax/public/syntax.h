/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/19 15:49:23 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "ft/libft.h"
# include "ft/tokenizer.h"

typedef struct s_syntax			t_syntax;
typedef struct s_syntax_scope	t_syntax_scope;

/*
** ========================================================================== **
** Syntax
*/

struct		s_syntax_scope
{
	t_sub		scope;
	t_syntax	*syntax;
};

struct		s_syntax
{
	t_syntax_scope	*end_token;
	t_token_map		token_map;
};

#endif
