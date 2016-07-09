/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:25:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/09 15:44:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "ft/libft.h"
# include "ft/parser.h"
# include "sh/parser.h"

typedef struct s_sh_parser_data	t_sh_parser_data;
typedef enum e_sh_parser_token	t_sh_parser_token;

/*
** ========================================================================== **
** -
** [n]>[ ]file
** [n]>|[ ]file
** [n]>>[ ]file
** [n]<[ ]file
** [n]<<[ ]word
** [n]<<-[ ]word
** [n]>&[ ]fd
** [n]>&[ ]-
** [n]<&[ ]fd
** [n]<&[ ]-
** [n]<>[ ]file
*/

enum		e_sh_parser_token
{
	SH_PARSE_T_NONE,
	SH_PARSE_T_SPACE,

	SH_PARSE_T_NEXT,

	SH_PARSE_T_ESCAPED,
	SH_PARSE_T_BACKSLASH,

	SH_PARSE_T_REDIR,

	SH_PARSE_T_EXPR,

	SH_PARSE_T_PARAM,
	SH_PARSE_T_PARAM_SPECIAL,
};

struct		s_sh_parser_data
{
	t_sh_parser_token	t;
	uint32_t			data;
};

bool			sh_parse_text(t_parse_data *p, t_sh_text *text);

bool			sh_parse_frame_cmd(t_parse_data *p);
bool			sh_parse_frame_cmd_compound(t_parse_data *p);

// bool			sh_parse_frame_math(t_parse_data *p);
bool			sh_parse_frame_string(t_parse_data *p);

bool			sh_parse_frame_expr(t_parse_data *p);
bool			sh_parse_frame_expr_val(t_parse_data *p);

bool			sh_parse_frame_ignore(t_parse_data *p);

#endif
