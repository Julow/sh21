/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:25:51 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 01:36:27 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ft/ft_dstr.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"

typedef struct s_sh_redir		t_sh_redir;
typedef struct s_sh_expr		t_sh_expr;
typedef struct s_sh_token		t_sh_token;
typedef struct s_sh_text		t_sh_text;
typedef struct s_sh_cmd			t_sh_cmd;
typedef enum e_sh_token_t		t_sh_token_t;

/*
** ========================================================================== **
** Shell command representation
*/

// struct		s_sh_redir
// {
// 	enum {
// 		SH_REDIR_INPUT, // [left_fd]<right
// 		SH_REDIR_OUTPUT, // [left_fd]>right
// 		// SH_REDIR_OUTPUT_CLOBBER, // [left_fd]>|right
// 		SH_REDIR_APPEND, // [left_fd]>>right
// 		SH_REDIR_HEREDOC, // [left_fd]<<right
// 	}			type;
// 	uint32_t	left_fd;
// 	enum {
// 		SH_REDIR_RIGHT_FD,
// 		SH_REDIR_RIGHT_TEXT,
// 	}			right_type;
// 	union {
// 		uint32_t	fd;
// 		uint32_t	file_len;
// 	}			right;
// 	t_sh_redir	*next;
// };

// struct		s_sh_expr
// {
// 	enum {
// 		SH_SUBST_EXPR_USE_DEF,
// 		SH_SUBST_EXPR_SET_DEF,
// 		SH_SUBST_EXPR_ISSET,
// 		SH_SUBST_EXPR_USE_ALT,
// 		SH_SUBST_EXPR_SUFFIX_PATTERN,
// 		SH_SUBST_EXPR_PREFIX_PATTERN,
// 		SH_SUBST_EXPR_F_COLON = (1 << 17),
// 		SH_SUBST_EXPR_F_LARGEST = (1 << 18),
// 	}			type;
// 	t_sh_text	expr;
// };

enum		e_sh_token_t
{
	SH_T_STRING,
	SH_T_STRING_QUOTED,
	SH_T_SPACE,
	// SH_T_REDIR,
	SH_T_SUBSHELL,
	SH_T_PARAM,
	SH_T_PARAMLEN,
	// SH_T_EXPR,
};

struct		s_sh_token
{
	t_sh_token_t	type;
	union {
		uint32_t	token_len;
		t_sh_cmd	*cmd;
		// t_sh_expr	*expr;
		// t_sh_redir	*redir;
	}			val;
};

struct		s_sh_text
{
	t_dstr		text;
	t_vector	tokens;
};

struct		s_sh_cmd
{
	t_sh_text	text;
	bool		async;
	enum {
		SH_NEXT_AND,
		SH_NEXT_OR,
		SH_NEXT_PIPE,
		SH_NEXT_NEW,
	}			next_type;
	t_sh_cmd	*next;
};

#endif
