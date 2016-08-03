/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:51:52 by juloo             #+#    #+#             */
/*   Updated: 2016/08/04 01:08:18 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_PARSER_H
# define P_SH_PARSER_H

# include "ft/lexer.h"
# include "sh/parser.h"

typedef struct s_sh_parse_token		t_sh_parse_token;
typedef struct s_sh_parser			t_sh_parser;

/*
** ========================================================================== **
*/

struct			s_sh_parse_token
{
	enum {
		SH_PARSE_T_SPACE,

		SH_PARSE_T_SUBST,
		SH_PARSE_T_STRING,
		SH_PARSE_T_COMMENT,

		SH_PARSE_T_COMPOUND_END,
		SH_PARSE_T_LIST_END,
		SH_PARSE_T_PIPELINE_END,

		SH_PARSE_T_BACKSLASH,
		SH_PARSE_T_ESCAPED,
		SH_PARSE_T_REDIR,
		SH_PARSE_T_HEREDOC,
	}				type;
	union {
		bool				_nil;

		enum {
			SH_PARSE_T_SUBST_EXPR,
			SH_PARSE_T_SUBST_MATH,
			SH_PARSE_T_SUBST_SUBSHELL,
			SH_PARSE_T_SUBST_BACKQUOTE,
			SH_PARSE_T_SUBST_DOLLAR,
		}					subst;

		enum {
			SH_PARSE_T_STRING_DOUBLE,
			SH_PARSE_T_STRING_SIMPLE,
			SH_PARSE_T_STRING_END,
		}					string;

		enum {
			SH_PARSE_T_END_SEMICOLON,
			SH_PARSE_T_END_AMPERSAND,
			SH_PARSE_T_END_NEWLINE,
		}					compound_end;

		t_sh_list_next_t	list_end;
		t_sh_redir_t		redir;
	};
};

struct			s_sh_parser
{
	t_lexer			l;
	t_sh_parse_err	*err;
};

# define SH_T(P)	((t_sh_parse_token const*)((P)->l.token))

# define SH_T_EQU(P,T)	(SH_T(P) != NULL && SH_T(P)->type == SH_PARSE_T_##T)

#endif