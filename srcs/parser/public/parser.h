/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/19 16:57:14 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "ft/regex.h"
# include "ft/tokenizer.h"

typedef struct s_parser			t_parser;
typedef struct s_parser_token	t_parser_token;
typedef struct s_parser_match	t_parser_match;
typedef struct s_parser_data	t_parser_data;

/*
** ========================================================================== **
** parser
*/

struct			s_parser_token
{
	void			*data;
	t_parser		*parser;
	bool			end;
};

struct			s_parser_match
{
	t_regex			regex;
	t_parser_token	token;
};

struct			s_parser
{
	void			*data;
	t_token_map		token_map;
	t_vector		match;
};

struct			s_parser_data
{
	void			*data;
	t_parser_data	*prev;
};

/*
** Execute parser on 'line'
** 'callback' is called after each token
** 'callback' is of type: void (*)(t_sub token, t_parser_data *data, void *env)
*/
void			exec_parser(t_sub line, void (*callback)(),
					t_parser const *parser, void *env);

#endif
