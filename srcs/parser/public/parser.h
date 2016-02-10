/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/10 19:13:58 by jaguillo         ###   ########.fr       */
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

typedef struct s_parse_data		t_parse_data;
typedef struct s_parse_frame	t_parse_frame;

/*
** ========================================================================== **
** Parsing
*/

struct			s_parse_frame
{
	t_parser const	*parser;
	void			*data;
	t_parse_frame	*prev;
};

/*
** Parse data
** -
** 'env'		User data
** 't'			Tokenizer
** 'frame'		The current frame
** TODO: 'eof'		Set if EOF is reached
** 'flags'		Flags:
** 			PARSE_F_FIRST	Set when parse_token return it's first token
** 'token'		The current token's string
** 'token_data'	The current token's data
*/

struct			s_parse_data
{
	void			*env;
	t_tokenizer		t;
	t_parse_frame	*frame;
	bool			eof;
	uint32_t		flags;
	t_sub			token;
	void const		*token_data;
};

# define PARSE_F_FIRST		(1 << 0)
# define _PARSE_F_FIRST		(1 << 1)

/*
** Start parsing
** Call 'f' function store in each parsers
** 'p->token' and 'p->token_data' are set to the begin token's datas
*/
bool			parse(t_in *in, t_parser const *parser, void *env);

/*
** Iterate over tokens
** 'p->eof' is set to true on EOF
** 'p->token' represent the token string
** 'p->token_data' represent the token data
** Return false when current frame should stop
** On returning false: 'p->token' and 'p->token_data'
**  are set to the end token's datas
*/
bool			parse_token(t_parse_data *p);

/*
** Create a new frame and execute 'parser'
*/
bool			parse_frame(t_parse_data *p, t_parser const *parser);

/*
** ========================================================================== **
** Parser
** -
** TODO: .tail		call next parser without recursion
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
	bool			(*f)(t_parse_data *);
	t_token_map		token_map;
	t_vector		match;
	bool			resolved;
};

#endif
