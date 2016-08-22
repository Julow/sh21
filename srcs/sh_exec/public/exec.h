/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/18 22:05:40 by juloo             #+#    #+#             */
/*   Updated: 2016/08/22 15:54:46 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "sh/ast.h"
# include "sh/context.h"

/*
** ========================================================================== **
** Sh exec
*/

/*
** Execute a compound command
** If 'no_fork' is true, reuse the current process and kill it
*/
int				sh_exec_compound(t_sh_context *c,
					t_sh_compound const *cmd, bool no_fork);

#endif
