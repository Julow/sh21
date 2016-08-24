/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/23 19:04:59 by juloo             #+#    #+#             */
/*   Updated: 2016/08/24 18:56:58 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "ft/ft_dstr.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"

typedef struct s_sh_expr		t_sh_expr;
typedef struct s_sh_heredoc		t_sh_heredoc;
typedef struct s_sh_token		t_sh_token;
typedef struct s_sh_text		t_sh_text;
typedef enum e_sh_token_t		t_sh_token_t;
typedef enum e_sh_expr_t		t_sh_expr_t;

typedef struct s_sh_simple		t_sh_simple;
typedef struct s_sh_subshell	t_sh_subshell;
typedef struct s_sh_for			t_sh_for;
typedef struct s_sh_else		t_sh_else;
typedef struct s_sh_if			t_sh_if;
typedef struct s_sh_while		t_sh_while;

typedef enum e_sh_redir_t		t_sh_redir_t;
typedef struct s_sh_redir		t_sh_redir;
typedef struct s_sh_redir_lst	t_sh_redir_lst;

typedef enum e_sh_cmd_t			t_sh_cmd_t;
typedef struct s_sh_cmd			t_sh_cmd;
typedef struct s_sh_pipeline	t_sh_pipeline;
typedef enum e_sh_list_next_t	t_sh_list_next_t;
typedef struct s_sh_list_next	t_sh_list_next;
typedef struct s_sh_list		t_sh_list;
typedef struct s_sh_compound	t_sh_compound;

/*
** ========================================================================== **
** Text
** (List of tokens)
*/

/*
** Token
*/

enum			e_sh_token_t
{
	SH_T_STRING = 1,
	SH_T_SPACE,
	SH_T_SUBSHELL,
	SH_T_PARAM,
	SH_T_PARAM_POS,
	SH_T_EXPR,
	SH_F_T_QUOTED = 1 << 24,
};

struct			s_sh_token
{
	t_sh_token_t	type;
	union {
		uint32_t		token_len;
		t_sh_compound	*cmd;
		uint32_t		param_len;
		uint32_t		param_pos;
		t_sh_expr		*expr;
	};
};

# define SH_TOKEN(T,V)	((t_sh_token){SH_T_##T, {V}})

/*
** Text
*/

struct			s_sh_text
{
	t_dstr			text;
	t_vector		tokens;
};

# define SH_TEXT()	((t_sh_text){DSTR0(), VECTOR(t_sh_token)})

/*
** Expr
*/

enum			e_sh_expr_t
{
	SH_EXPR_NONE = 0,
	SH_EXPR_USE_DEF,
	SH_EXPR_SET_DEF,
	SH_EXPR_ISSET,
	SH_EXPR_USE_ALT,
	SH_EXPR_SUFFIX,
	SH_EXPR_PREFIX,
	SH_EXPR_F_ALT = (1 << 8),
};

struct			s_sh_expr
{
	t_sh_expr_t		type;
	uint32_t		param_len;
	t_sh_text		text;
};

#define SH_EXPR(L)	((t_sh_expr){SH_EXPR_NONE, (L), SH_TEXT()})

/*
** ========================================================================== **
** Sh syntax tree
*/

/*
** Simple commands
*/

struct			s_sh_simple
{
	t_sh_text		text;
};

/*
** Redir
*/

enum			e_sh_redir_t
{
	SH_REDIR_OUTPUT,			// [n]>word		// 1
	SH_REDIR_OUTPUT_CLOBBER,	// [n]>|word	// 1
	SH_REDIR_APPEND,			// [n]>>word	// 1
	SH_REDIR_INPUT,				// [n]<word		// 0
	SH_REDIR_INPUT_FD,			// [n]<&word	// 0
	// SH_REDIR_INPUT_CLOSE		// [n]<&-		// 0
	SH_REDIR_OUTPUT_FD,			// [n]>&word	// 1
	// SH_REDIR_OUTPUT_CLOSE,	// [n]>&-		// 1
	SH_REDIR_OPEN,				// [n]<>word	// 1
};

// TODO: share 'right_text' text object with all redirections
struct			s_sh_redir
{
	int32_t			left_fd;
	t_sh_text		right_text;
	t_sh_redir_t	type;
};

# define SH_REDIR(TYPE, LEFT)	((t_sh_redir){(LEFT), SH_TEXT(), (TYPE)})

struct			s_sh_redir_lst
{
	t_vector		redirs;
};

# define SH_REDIR_LST()			((t_sh_redir_lst){VECTOR(t_sh_redir)})

// struct			s_sh_heredoc
// {
// 	enum {
// 		SH_HEREDOC_F_STRIP_TABS = 1 << 0,
// 		SH_HEREDOC_F_READ = 1 << 1,
// 	}			flags;
// 	t_sh_text	text;
// };

/*
** Pipeline
*/

enum			e_sh_cmd_t
{
	SH_CMD_EMPTY,
	SH_CMD_SIMPLE,
	SH_CMD_SUBSHELL,
	SH_CMD_IF_CLAUSE,
	SH_CMD_FOR_CLAUSE,
	SH_CMD_WHILE_CLAUSE, // while_clause
	SH_CMD_UNTIL_CLAUSE, // while_clause
	SH_CMD_TIME_CLAUSE, // rec
	SH_CMD_NOT_CLAUSE, // rec
	SH_CMD_BRACKET_CLAUSE,
};

struct			s_sh_cmd
{
	t_sh_cmd_t		type;
	t_sh_redir_lst	redirs;
	union {
		t_sh_simple		simple;
		t_sh_subshell	*subshell;
		t_sh_if			*if_clause;
		t_sh_for		*for_clause;
		t_sh_while		*while_clause;
		t_sh_cmd		*rec;
		t_sh_compound	*bracket_clause;
	};
};

struct			s_sh_pipeline
{
	t_sh_cmd		cmd;
	t_sh_pipeline	*next;
};

/*
** List
*/

struct			s_sh_list
{
	t_sh_pipeline	pipeline;
	t_sh_list_next	*next;
};

enum			e_sh_list_next_t
{
	SH_LIST_AND,
	SH_LIST_OR,
};

struct			s_sh_list_next
{
	t_sh_list			next;
	t_sh_list_next_t	type;
};

/*
** Compound list
*/

struct			s_sh_compound
{
	t_sh_list		list;
	enum {
		SH_COMPOUND_ASYNC = 1 << 0,
	}				flags;
	t_sh_compound	*next;
};

/*
** Simple commands
*/

struct			s_sh_subshell
{
	t_sh_compound	compound;
};

struct			s_sh_for
{
	t_sub			var;
	t_sh_text		data;
	t_sh_compound	body;
};

struct			s_sh_if
{
	t_sh_compound	cond;
	t_sh_compound	body;
	t_sh_else		*else_clause;
};

struct			s_sh_else
{
	enum {
		SH_ELSE_ELIF,
		SH_ELSE_ELSE,
	}				type;
	union {
		t_sh_if			elif_clause;
		t_sh_compound	else_clause;
	};
};

struct			s_sh_while
{
	t_sh_compound	cond;
	t_sh_compound	body;
};

/*
** ========================================================================== **
** Clean functions
*/

/*
** 'c' and 'text' pointers are not freed
*/
void			sh_destroy_redir_lst(t_sh_redir_lst *lst);
void			sh_destroy_compound(t_sh_compound *c);
void			sh_destroy_cmd(t_sh_cmd *c);
void			sh_destroy_text(t_sh_text *text);

#endif
