/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:51:52 by juloo             #+#    #+#             */
/*   Updated: 2016/07/29 01:37:40 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_PARSER_H
# define P_SH_PARSER_H

# include "ft/parser.h"
# include "sh/parser.h"

typedef struct s_sh_token_data		t_sh_token_data;
typedef struct s_sh_parse_data		t_sh_parse_data;

/*
** ========================================================================== **
*/

struct			s_sh_token_data
{
	enum {
		SH_PARSE_T_NONE,
		SH_PARSE_T_SPACE,
		SH_PARSE_T_END,
		SH_PARSE_T_ESCAPED,

		SH_PARSE_T_BACKSLASH,
		SH_PARSE_T_REDIR,
		SH_PARSE_T_HEREDOC,
	}				type;
	union {
		bool			none;
		enum {
			SH_PARSE_T_END_AND,
			SH_PARSE_T_END_OR,
			SH_PARSE_T_END_PIPE,
			SH_PARSE_T_END_SEMICOLON,
			SH_PARSE_T_END_AMPERSAND,
			SH_PARSE_T_END_NEWLINE,
		}				end;
		t_sh_redir_t	redir;
	};
};

struct			s_sh_parse_data
{
	t_sh_parse_err	*err;
};

# define SH_PARSE_DATA(ERR)	((t_sh_parse_data){(ERR)})

t_parser const	*sh_load_parser(t_sub key);

bool			sh_parse_frame_compound(t_parse_data *p,
					void const *data, t_sh_compound *dst);

#endif
