/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/19 14:48:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "ft/regex.h"
# include "ft/tokenizer.h"

typedef struct s_syntax			t_syntax;
typedef struct s_syntax_token	t_syntax_token;
typedef struct s_syntax_match	t_syntax_match;
typedef struct s_syntax_data	t_syntax_data;

/*
** ========================================================================== **
** Syntax
*/

struct			s_syntax_token
{
	void			*data;
	t_syntax		*syntax;
	bool			end;
};

struct			s_syntax_match
{
	t_regex			regex;
	t_syntax_token	token;
};

struct			s_syntax
{
	void			*data;
	t_token_map		token_map;
	t_vector		match;
};

struct			s_syntax_data
{
	void			*data;
	t_syntax_data	*prev;
};

/*
** Execute syntax on 'line'
** 'callback' is called after each token
** 'callback' is of type: void (*)(t_sub token, t_syntax_data *data, void *env)
*/
void			exec_syntax(t_sub line, void (*callback)(),
					t_syntax const *syntax, void *env);

#endif
