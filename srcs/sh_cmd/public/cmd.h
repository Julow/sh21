/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:25:51 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/12 19:24:34 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "ft/ft_dstr.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"

typedef struct s_sh_expr		t_sh_expr;
typedef struct s_sh_token		t_sh_token;
typedef struct s_sh_text		t_sh_text;
typedef struct s_sh_cmd			t_sh_cmd;
typedef enum e_sh_token_t		t_sh_token_t;
typedef enum e_sh_redir_t		t_sh_redir_t;
typedef enum e_sh_expr_t		t_sh_expr_t;

/*
** ========================================================================== **
** Shell cmd
*/

enum		e_sh_redir_t
{
	SH_REDIR_OUTPUT,			// [n]>word		// 1
	SH_REDIR_OUTPUT_CLOBBER,	// [n]>|word	// 1
	SH_REDIR_APPEND,			// [n]>>word	// 1
	SH_REDIR_INPUT,				// [n]<word		// 0
	SH_REDIR_HEREDOC,			// [n]<<word	// 0
	SH_REDIR_INPUT_FD,			// [n]<&word	// 0
	// SH_REDIR_INPUT_CLOSE		// [n]<&-		// 0
	SH_REDIR_OUTPUT_FD,			// [n]>&word	// 1
	// SH_REDIR_OUTPUT_CLOSE,	// [n]>&-		// 1
	SH_REDIR_OPEN,				// [n]<>word	// 1
};

/*
** Token type
** SH_F_T_QUOTED is an extra flag
** Quoting space or redir make no sense and can cause undefined behavior
*/
enum		e_sh_token_t
{
	SH_T_STRING = 1,
	SH_T_SPACE,
	SH_T_REDIR,
	SH_T_SUBSHELL,
	SH_T_PARAM,
	SH_T_EXPR,
	SH_F_T_QUOTED = 1 << 24,
};

struct		s_sh_token
{
	t_sh_token_t	type;
	union {
		uint32_t		token_len;
		t_sh_cmd		*cmd;
		t_sh_redir_t	redir_type;
		t_sh_expr		*expr;
	}				val;
};

struct		s_sh_text
{
	t_dstr		text;
	t_vector	tokens;
};

struct		s_sh_cmd
{
	t_sh_text	text;
	enum {
		SH_NEXT_AND,
		SH_NEXT_OR,
		SH_NEXT_PIPE,
		SH_NEXT_SEQ,
		SH_NEXT_ASYNC,
	}			next_type;
	t_sh_cmd	*next;
};

enum		e_sh_expr_t
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

struct		s_sh_expr
{
	t_sh_expr_t	type;
	uint32_t	param_len;
	t_sh_text	text;
};

/*
** ========================================================================== **
*/

#define SH_TEXT()	((t_sh_text){DSTR0(), VECTOR(t_sh_token)})
#define SH_CMD()	((t_sh_cmd){SH_TEXT(), SH_NEXT_SEQ, NULL})
#define SH_EXPR(L)	((t_sh_expr){SH_EXPR_NONE, (L), SH_TEXT()})

/*
** Destroy a t_sh_text
** The 'text' ptr is NOT freed
*/
void		sh_destroy_text(t_sh_text *text);

/*
** Destroy a t_sh_text
** The 'expr' ptr is freed
*/
void		sh_destroy_expr(t_sh_expr *expr);

/*
** Destroy a t_sh_cmd
** The 'cmd' ptr IS freed
*/
void		sh_destroy_cmd(t_sh_cmd *cmd);

#endif
