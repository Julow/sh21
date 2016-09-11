/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_default_builtins.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:11:27 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/11 19:09:07 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_DEFAULT_BUILTINS_H
# define P_DEFAULT_BUILTINS_H

# include "sh/default_builtins.h"

typedef struct s_sh_builtin_def		t_sh_builtin_def;

/*
** ========================================================================== **
*/

struct			s_sh_builtin_def
{
	t_sub			name;
	t_sh_builtin	builtin_f;
};

int				sh_builtin_echo(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_true(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_false(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_exit(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_pwd(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_set(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_unset(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_shift(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_cd(t_sh_context *c, void *data, t_argv args);
int				sh_builtin_source(t_sh_context *c, void *data, t_argv args);

#endif
