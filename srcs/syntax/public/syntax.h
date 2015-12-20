/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/20 00:05:42 by juloo            ###   ########.fr       */
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

struct			s_syntax_scope
{
	t_sub			name;
	t_syntax		*syntax;
};

struct			s_syntax
{
	t_sub			scope;
	t_syntax_scope	*end_token; // should be a scope name
	t_token_map		token_map;
};

/*
** Execute syntax on 'line'
** 'callback' is called after each token
** 'callback' is of type: void (*)(t_sub token, t_sub scope, void *env)
*/
void			exec_syntax(t_sub line, void (*callback)(),
					t_syntax const *syntax, void *env);

#endif
