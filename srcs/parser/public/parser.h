/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/24 21:47:03 by juloo            ###   ########.fr       */
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
	t_parser_data	*next;
};

/*
** Execute parser on 'line'
** callbacks[0]:
**  void (*)(void *env, t_parser_data *data, void const *parser_data)
** callbacks[1]:
**  void (*)(void *env, t_parser_data *data, void const *parser_data)
** callbacks[2]:
**  void (*)(void *env, t_parser_data *parent, t_sub token, void const *data)
*/
void			exec_parser(t_sub line, t_parser const *parser,
					t_callback callbacks[3], uint32_t data_size);

#endif
