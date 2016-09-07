/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_default_builtins.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:11:27 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 16:33:29 by jaguillo         ###   ########.fr       */
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

// TODO: builtin function & context builtin manipulation: prefix collision

int				sh_builtin_echo(t_sh_context *c, void *data,
					uint32_t argc, char const *const *argv);

#endif
