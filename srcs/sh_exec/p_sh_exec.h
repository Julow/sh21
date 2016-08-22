/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/18 22:10:40 by juloo             #+#    #+#             */
/*   Updated: 2016/08/22 20:59:31 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_EXEC_H
# define P_SH_EXEC_H

# include "ft/ft_dstr.h"
# include "sh/ast.h"
# include "sh/context.h"
# include "sh/exec.h"

# include <sys/types.h>

/*
** ========================================================================== **
*/

int				sh_exec_list(t_sh_context *c,
					t_sh_list const *cmd, bool no_fork);

int				sh_exec_pipeline(t_sh_context *c,
					t_sh_pipeline const *cmd, bool no_fork);

/*
** If 'no_fork' is true use current process and exit
** otherwise fork if needed and wait for child process to complete
** 'fd' is the pair <stdin, stdout>
*/
extern int		(*const g_sh_exec_cmd[])(t_sh_context *c, t_sh_cmd const *cmd,
					bool no_fork);

/*
** -
*/

int				sh_exec_cmd_simple(t_sh_context *c, t_sh_cmd const *cmd,
					bool no_fork);

/*
** Execute a t_sh_text
** Append the text into 'dst', params are separated by '\0'
** If 'first_quoted' is not NULL,
**  put true if the first param is quoted, false otherwise
** Return the number of param
*/
uint32_t		sh_exec_text(t_sh_context *c, t_sh_text const *text,
					t_dstr *dst, bool *first_quoted);

/*
** -
*/

/*
** Search for a command in the PATH
** 'dst' content is set to the full path of the binary
*/
bool			sh_search_path(t_sh_context const *c, t_sub name, t_dstr *dst);

/*
** Wait for a process and return it's status code
*/
int				sh_wait_pid(t_sh_context *c, pid_t pid);

#endif
