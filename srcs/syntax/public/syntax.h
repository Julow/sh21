/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/08 16:40:48 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "ft/regex.h"
# include "ft/tokenizer.h"

typedef struct s_syntax			t_syntax;
typedef struct s_syntax_scope	t_syntax_scope;
typedef struct s_syntax_match	t_syntax_match;

/*
** ========================================================================== **
** Syntax
*/

struct			s_syntax_scope
{
	t_sub			name;
	t_syntax		*syntax;
	bool			end;
};

struct			s_syntax_match
{
	t_regex			regex;
	t_syntax_scope	*scope;
};

struct			s_syntax
{
	t_sub			scope;
	t_token_map		token_map;
	t_vector		match;
};

/*
** Execute syntax on 'line'
** 'callback' is called after each token
** 'callback' is of type: void (*)(t_sub token, t_sub scope, void *env)
*/
void			exec_syntax(t_sub line, void (*callback)(),
					t_syntax const *syntax, void *env);

#endif
