/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:51:52 by juloo             #+#    #+#             */
/*   Updated: 2016/09/14 15:11:44 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_PARSER_H
# define P_SH_PARSER_H

# include "ft/lexer.h"
# include "ft/tokenizer.h"
# include "sh/parser.h"

typedef struct s_sh_parse_token		t_sh_parse_token;
typedef struct s_sh_parser			t_sh_parser;
typedef struct s_sh_parser_heredoc	t_sh_parser_heredoc;

/*
** ========================================================================== **
*/

struct			s_sh_parse_token
{
	enum {
		SH_PARSE_T_SPACE,
		SH_PARSE_T_TEXT,

		SH_PARSE_T_PARAM,
		SH_PARSE_T_PARAM_LENGTH,
		SH_PARSE_T_PARAM_POS,
		SH_PARSE_T_PARAM_SPECIAL,

		SH_PARSE_T_SUBST,
		SH_PARSE_T_SUBSHELL,
		SH_PARSE_T_SUBST_PARAM_OP,
		SH_PARSE_T_SUBST_PARAM_END,

		SH_PARSE_T_STRING,
		SH_PARSE_T_COMMENT,

		SH_PARSE_T_PARENTHESIS_OPEN,

		SH_PARSE_T_COMPOUND_END,
		SH_PARSE_T_LIST_END,
		SH_PARSE_T_PIPELINE_END,

		SH_PARSE_T_BACKSLASH,
		SH_PARSE_T_ESCAPED,
		SH_PARSE_T_ESCAPE_SEQUENCE,
		SH_PARSE_T_HEREDOC,
		SH_PARSE_T_REDIR,
		_SH_PARSE_T_COUNT_
	}				type;
	union {
		uint32_t			_val;

		uint32_t			param_prefix;
		t_sh_special_param	param_special;

		enum {
			SH_PARSE_T_SUBST_PARAM,
			SH_PARSE_T_SUBST_MATH,
		}					subst;

		enum {
			SH_PARSE_T_SUBSHELL_BRACE,
			SH_PARSE_T_SUBSHELL_BACKQUOTE,
			SH_PARSE_T_SUBSHELL_BACKQUOTE_REV,
		}					subshell;

		t_sh_subst_param_t	subst_param_op;

		enum {
			SH_PARSE_T_STRING_NORMAL,
			SH_PARSE_T_STRING_SIMPLE,
			SH_PARSE_T_STRING_ANSI,
			SH_PARSE_T_STRING_END,
		}					string;

		enum {
			SH_PARSE_T_COMPOUND_SEMICOLON,
			SH_PARSE_T_COMPOUND_AMPERSAND,
			SH_PARSE_T_COMPOUND_NEWLINE,
			SH_PARSE_T_COMPOUND_SUBSHELL,
			SH_PARSE_T_COMPOUND_DO,
			SH_PARSE_T_COMPOUND_DONE,
			SH_PARSE_T_COMPOUND_THEN,
			SH_PARSE_T_COMPOUND_ELSE,
			SH_PARSE_T_COMPOUND_ELIF,
			SH_PARSE_T_COMPOUND_FI,
			SH_PARSE_T_COMPOUND_BRACKET,
		}					compound_end;

		t_sh_list_next_t	list_end;

		char				escaped;
		enum {
			SH_PARSE_T_ESCAPE_OCTAL,
			SH_PARSE_T_ESCAPE_HEXA,
			SH_PARSE_T_ESCAPE_CONTROL,
		}					escape_sequence;

		bool				heredoc_strip;
		t_sh_redir_t		redir;
	};
};

struct			s_sh_parser
{
	t_tokenizer			t;
	t_sh_parse_err		*err;
	t_sh_parser_heredoc	*heredoc_lst;
};

struct			s_sh_parser_heredoc
{
	t_sh_parser_heredoc	*next;
	t_sub				end;
	enum {
		SH_HEREDOC_QUOTED = 1 << 0,
		SH_HEREDOC_STRIP = 1 << 1,
	}					flags;
	t_sh_text			*dst;
};

# define T(T, ...)			(&SH_PARSE_T(T, ##__VA_ARGS__))

# define SH_PARSE_T(T, ...)	((t_sh_parse_token){SH_PARSE_T_##T, {__VA_ARGS__}})

# define SH(T)				SH_PARSE_T_##T

# define SH_T(P)			((t_sh_parse_token const*)((P)->t.token))

# define SH_T_EQU(P,T,V)	(SH_T(P)->type == SH(T) && SH_T(P)->_val == SH(V))

# define SH_T_EXCEPT(P,T,V)	\
		((P)->t.eof ? (sh_parse_error(P, SH_E_EOF), false) : \
			(SH_T_EQU(P,T,V) || (sh_parse_error(P, SH_E_UNEXPECTED), false)))

/*
** -
*/

extern t_lexer_def const	g_sh_text_string_lexer_base;
extern t_lexer_def const	g_sh_lexer_base;

/*
** -
*/

bool			sh_parse_compound(t_sh_parser *p, t_sh_compound *dst,
					bool allow_newline);

bool			sh_parse_cmd(t_sh_parser *p, t_sh_cmd *cmd);

bool			sh_parse_if_clause(t_sh_parser *p, t_sh_cmd *dst);
bool			sh_parse_for_clause(t_sh_parser *p, t_sh_cmd *dst);
bool			sh_parse_while_clause(t_sh_parser *p, t_sh_cmd *dst);
bool			sh_parse_do_clause(t_sh_parser *p, t_sh_compound *dst);

/*
** -
*/

bool			sh_ignore_spaces(t_sh_parser *p);
bool			sh_ignore_newlines(t_sh_parser *p);
bool			sh_except_token(t_sh_parser *p, t_sh_parse_token t);

bool			sh_parse_heredoc_lst(t_sh_parser *p);

/*
** Create error object
** Return false
*/
bool			sh_parse_error(t_sh_parser *p, t_sh_parse_err_t err);
bool			sh_parse_error_unterminated(t_sh_parser *p,
					t_sh_parse_err_unterminated t);

void			sh_text_push(t_sh_text *text, t_sub str, t_sh_token t, bool quoted);
void			sh_text_push_string(t_sh_text *text, t_sub str, bool quoted);

/*
** Parse compound end token (do, done, then, fi, else, ...)
*/
bool			sh_parse_compound_end(t_sh_parser *p);

/*
** Parse a redirection
** The current token must be a REDIR token
*/
bool			sh_parse_redir(t_sh_parser *p, t_sh_redir_lst *lst);
bool			sh_parse_redir_heredoc(t_sh_parser *p, t_sh_redir *dst);

/*
** Parse a redirection
** The current token must be a TEXT token
** Return false if the current token is invalid or the ahead is not a REDIR
** otherwise return true and put the error status in 'r'
*/
bool			sh_parse_redir_left(t_sh_parser *p,
					t_sh_redir_lst *lst, bool *r);

/*
** Parse trailing redirections until END token
** Do not accept any other token
*/
bool			sh_parse_trailing_redirs(t_sh_parser *p, t_sh_redir_lst *lst);

/*
** -
*/

/*
** Parse functions for text tokens
** Non-text tokens are NULL
*/
extern bool		(*const g_sh_parse_text[])(t_sh_parser *p, t_sh_text *dst, bool quoted);

bool			sh_parse_text_comment(t_sh_parser *p,
					t_sh_text *dst, bool quoted);
bool			sh_parse_text_string(t_sh_parser *p,
					t_sh_text *dst, bool quoted);
bool			sh_parse_text_subshell(t_sh_parser *p,
					t_sh_text *dst, bool quoted);
bool			sh_parse_text_escape_sequence(t_sh_parser *p,
					t_sh_text *dst, bool quoted);
bool			sh_parse_text_subst_param(t_sh_parser *p,
					t_sh_text *dst, bool quoted);

#endif
