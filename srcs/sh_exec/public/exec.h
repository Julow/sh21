/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/05 20:46:11 by juloo             #+#    #+#             */
/*   Updated: 2016/07/11 13:35:52 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ft/libft.h"
# include "sh/cmd.h"
# include "sh/context.h"

/*
** ========================================================================== **
** Sh exec
*/

/*
** Execute a compound list of command
** Return the return status of the last command
*/
int				sh_exec_cmd(t_sh_context *context, t_sh_cmd const *cmd);

#endif
