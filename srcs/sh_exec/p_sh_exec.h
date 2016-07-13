/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 15:38:31 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/13 12:07:36 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_EXEC_H
# define P_SH_EXEC_H

# include "ft/ft_dstr.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "sh/exec.h"

typedef struct s_sh_exec_redir		t_sh_exec_redir;
typedef struct s_sh_exec_arg		t_sh_exec_arg;
typedef struct s_sh_exec			t_sh_exec;
typedef struct s_sh_exec_builder	t_sh_exec_builder;

/*
** ========================================================================== **
*/

/*
** An argument
** offset		=> start offset in t_sh_args.buff
** quoted		=> partially or totally quoted
*/
struct			s_sh_exec_arg
{
	uint32_t		offset:31;
	bool			quoted:1;
};

/*
** A redirection
*/
struct			s_sh_exec_redir
{
	t_vec2u			fd;
	uint32_t		flags;
};

# define SH_REDIR_F_OPENED	(1 << 0)
# define SH_REDIR_F_CLOSE	(1 << 1)

/*
** Represent a command to execute
** buff			=> text
** args			=> vector of t_sh_exec_arg
** redirs		=> vector of t_sh_exec_redir
*/
struct			s_sh_exec
{
	t_dstr			buff;
	t_vector		args;
	t_vector		redirs;
};

# define SH_EXEC_ARG(EXEC)	((t_sh_exec_arg){(EXEC)->buff.length, false})

/*
** Build a sh_exec object from a command's text
** Mutually recursive with sh_exec_cmd
** TODO: check fail
*/
bool			build_sh_exec(t_sh_context *context, t_sh_text const *text,
					t_sh_exec *dst);

/*
** Exec a binary
*/
int				exec_binary(t_sh_context *context, t_sh_exec const *exec);

/*
** Destroy a sh_exec object
*/
void			destroy_sh_exec(t_sh_exec *exec);

/*
** sh_exec builder
*/

struct			s_sh_exec_builder
{
	t_sh_context	*context;
	t_sh_text const	*text;
	uint32_t		text_i;
	uint32_t		token_i;
	t_sh_exec_arg	arg;
	t_sh_exec		*dst;
};

bool			build_args_expr(t_sh_exec_builder *b, t_sh_token const *t);
bool			build_args_subshell(t_sh_exec_builder *b, t_sh_token const *t);
bool			build_args_redir(t_sh_exec_builder *b, t_sh_token const *t);
bool			build_args_string(t_sh_exec_builder *b, t_sh_token const *t);
bool			build_args_param(t_sh_exec_builder *b, t_sh_token const *t);
bool			build_args_space(t_sh_exec_builder *b, t_sh_token const *t);

void			build_sh_exec_push(t_sh_exec_builder *b);
bool			build_sh_exec_next(t_sh_exec_builder *b);

#endif
