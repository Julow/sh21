/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 13:40:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 01:36:58 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "sh/parser.h"

typedef enum e_sh_parser_token	t_sh_parser_token;

enum		e_sh_parser_token
{
	SH_PARSE_T_NONE = 0,
	SH_PARSE_T_AND,
	SH_PARSE_T_OR,
	SH_PARSE_T_PIPE,
	SH_PARSE_T_SEMICOLON,
	SH_PARSE_T_AMPERSAND,
	SH_PARSE_T_NEWLINE,
//
	SH_PARSE_T_SPACE,
	SH_PARSE_T_ESCAPED,
//
	SH_PARSE_T_PARAM,
	SH_PARSE_T_PARAM_SPECIAL,
};

#endif
