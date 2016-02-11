/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 13:40:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 13:41:23 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "sh/parser.h"

typedef enum e_sh_token			t_sh_token;
typedef enum e_sh_parser		t_sh_parser;

enum		e_sh_token
{
	SH_T_AND = 1,
	SH_T_OR,
	SH_T_PIPE,
	SH_T_SEMICOLON,
	SH_T_AMPERSAND,
	SH_T_NEWLINE,
//
	SH_T_SPACE,
	SH_T_BEGIN,
	SH_T_END,
	SH_T_ESCAPED,
//
	SH_T_SUBST_PARAM,
	SH_T_SUBST_PARAM_SPECIAL,
};

enum		e_sh_parser
{
	SH_P_SHELL = 1,
	SH_P_SUBSHELL,
	SH_P_BACKQUOTE,
	SH_P_EXPR,
	SH_P_MATH,
	SH_P_STRING,
	SH_P_STRING_SINGLE,
	SH_P_IGNORE,
};

#endif
