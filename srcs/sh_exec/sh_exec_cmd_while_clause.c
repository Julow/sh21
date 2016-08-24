/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_cmd_while_clause.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/25 01:04:26 by juloo             #+#    #+#             */
/*   Updated: 2016/08/25 01:30:17 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <stdlib.h>

static int		while_clause(t_sh_context *c, t_sh_cmd const *cmd,
					bool no_fork, bool until)
{
	uint32_t		saved_fds[cmd->redirs.redirs.length];
	int				status;

	if (!sh_exec_redir(c, &cmd->redirs, no_fork ? NULL : saved_fds))
		return (1);
	status = 0;
	while ((bool)sh_exec_compound(c, &cmd->while_clause->cond, false) != until)
		status = sh_exec_compound(c, &cmd->while_clause->body, false);
	if (no_fork)
		exit(status);
	sh_exec_redir_restore(&cmd->redirs, saved_fds);
	return (status);
}

int				sh_exec_cmd_while_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	return (while_clause(c, cmd, no_fork, true));
}

int				sh_exec_cmd_until_clause(t_sh_context *c,
					t_sh_cmd const *cmd, bool no_fork)
{
	return (while_clause(c, cmd, no_fork, false));
}
