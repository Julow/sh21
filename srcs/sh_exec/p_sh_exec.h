/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 15:38:31 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/06 17:06:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_EXEC_H
# define P_SH_EXEC_H

# include "ft/ft_dstr.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "sh/exec.h"

typedef struct s_sh_exec_arg	t_sh_exec_arg;
typedef t_sh_redir_t			t_sh_exec_redir; // TODO
typedef struct s_sh_exec		t_sh_exec;

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
*/
void			build_sh_exec(t_sh_context *context, t_sh_text const *text,
					t_sh_exec *dst);

/*
** Destroy a sh_exec object
*/
void			destroy_sh_exec(t_sh_exec *exec);

#endif
