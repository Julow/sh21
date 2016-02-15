/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:25:51 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/15 10:46:07 by jaguillo         ###   ########.fr       */
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
typedef enum e_sh_redir_t		t_sh_redir_t;

/*
** ========================================================================== **
** Shell command representation
*/

enum		e_sh_redir_t
{
	SH_REDIR_OUTPUT,
	SH_REDIR_OUTPUT_CLOBBER,
	SH_REDIR_APPEND,
	SH_REDIR_INPUT,
	SH_REDIR_HEREDOC,
	SH_REDIR_DUP_INPUT,
	SH_REDIR_DUP_OUTPUT,
	SH_REDIR_OPEN,
};

enum		e_sh_token_t
{
	SH_T_STRING,
	SH_T_STRING_QUOTED,
	SH_T_SPACE,
	SH_T_REDIR,
	SH_T_SUBSHELL,
	SH_T_PARAM,
	SH_T_PARAMLEN,
};

struct		s_sh_token
{
	t_sh_token_t	type;
	union {
		uint32_t		token_len;
		t_sh_cmd		*cmd;
		t_sh_redir_t	redir_type;
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
	bool		async;
	enum {
		SH_NEXT_AND,
		SH_NEXT_OR,
		SH_NEXT_PIPE,
		SH_NEXT_NEW,
	}			next_type;
	t_sh_cmd	*next;
};

#define SH_TEXT()	((t_sh_text){DSTR0(), VECTOR(t_sh_token)})
#define SH_CMD()	((t_sh_cmd){SH_TEXT(), false, SH_NEXT_NEW, NULL})

/*
** ========================================================================== **
*/

/*
** Destroy a t_sh_text
** The 'text' ptr is NOT freed
*/
void		sh_destroy_text(t_sh_text *text);

/*
** Destroy a t_sh_cmd
** The 'cmd' ptr IS freed
*/
void		sh_destroy_cmd(t_sh_cmd *cmd);

#endif
