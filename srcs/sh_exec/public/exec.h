/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:46:11 by juloo             #+#    #+#             */
/*   Updated: 2016/07/06 17:52:29 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ft/ft_hmap.h"
# include "ft/libft.h"
# include "sh/cmd.h"

typedef struct s_sh_context		t_sh_context;

/*
** ========================================================================== **
** Sh exec
*/

/*
** Context in which commands are executed
** env			=> env variables that are passed to programs
** var			=> other definitions (TODO)
*/
struct			s_sh_context
{
	char			*env;
	t_hmap			var;
};

/*
** Execute a compound list of command
** Return the return status of the last command
*/
int				sh_exec_cmd(t_sh_context *context, t_sh_cmd const *cmd);

#endif
