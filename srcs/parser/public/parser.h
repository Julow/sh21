/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/16 16:01:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/10 00:31:25 by juloo            ###   ########.fr       */
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
	t_parser const	*parser; // current parser
	void			*data; // custom frame data (default NULL)
	t_parse_frame	*prev; // previous frame
};

struct			s_parse_data
{
	void			*env; // custom data
	t_tokenizer		t; // tokenizer
	t_parse_frame	*frame; // current frame
	bool			eof; // flags
	// TODO: error flag + error info report
};

/*
** Start parsing
** call 'f' function store in each parsers
*/
bool			parse(t_in *in, t_parser const *parser, void *env);

/*
** Iterate over tokens
** Return false when current frame should stop
** 'p->eof' is set on EOF
** '*token_str' and '*token_data' are fill with token's infos
** Note: token_str string is temporary and is invalidated after each call
** '*token_data' is set to NULL on unmatched token
** 'token_str' and 'token_data' can be NULL
*/
bool			parse_token(t_parse_data *p,
					t_sub *token_str, void const **token_data);

/*
** ========================================================================== **
** Parser
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
