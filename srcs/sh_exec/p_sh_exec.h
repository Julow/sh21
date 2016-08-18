/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/18 22:10:40 by juloo             #+#    #+#             */
/*   Updated: 2016/08/18 22:14:15 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_EXEC_H
# define P_SH_EXEC_H

# include "sh/ast.h"
# include "sh/context.h"
# include "sh/exec.h"

/*
** ========================================================================== **
*/

int				sh_exec_compound(t_sh_context *c, t_sh_compound const *cmd);
int				sh_exec_list(t_sh_context *c, t_sh_list const *cmd);
int				sh_exec_pipeline(t_sh_context *c, t_sh_pipeline const *cmd);
int				sh_exec_cmd(t_sh_context *c, t_sh_cmd const *cmd);

#endif
