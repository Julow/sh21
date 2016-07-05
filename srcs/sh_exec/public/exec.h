/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:46:11 by juloo             #+#    #+#             */
/*   Updated: 2016/07/05 21:01:46 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ft/ft_dstr.h"
# include "ft/ft_hmap.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "sh/cmd.h"

typedef struct s_sh_context		t_sh_context;
typedef struct s_sh_args		t_sh_args;

/*
** ========================================================================== **
** Sh exec
*/

struct			s_sh_context
{
	char			*env;
	t_hmap			var;
	// TODO: stdout fd (dup)
};

struct			s_sh_args
{
	t_dstr			buff;
	t_vector		args;
};

/*
** Execute a compound list of command
** Return the return status of the last command
*/
int				sh_exec_cmd(t_sh_context *context, t_sh_cmd const *cmd);

#endif
