/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:25:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 12:32:49 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "ft/libft.h"
# include "ft/parser.h"
# include "sh/parser.h"

typedef enum e_sh_parser_token	t_sh_parser_token;

/*
** ========================================================================== **
*/

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
	SH_PARSE_T_BACKSLASH,
//
	SH_PARSE_T_PARAM,
	SH_PARSE_T_PARAM_SPECIAL,
};

bool			sh_parse_text(t_parse_data *p, t_sh_text *text);

bool			sh_parse_frame_cmd(t_parse_data *p);
bool			sh_parse_frame_sub(t_parse_data *p);
// bool			sh_parse_frame_expr(t_parse_data *p);
// bool			sh_parse_frame_math(t_parse_data *p);
bool			sh_parse_frame_string(t_parse_data *p);

bool			sh_parse_frame_ignore(t_parse_data *p);

#endif
