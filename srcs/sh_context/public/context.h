/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 10:25:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 22:32:01 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "ft/libft.h"
# include "ft/set.h"
# include "ft/strset.h"

typedef struct s_sh_context		t_sh_context;

/*
** ========================================================================== **
** Sh context
*/

/*
** Context in which commands are executed
** vars				=> variables
** env_keys			=> key of variables in env
*/
struct			s_sh_context
{
	t_set			vars;
	t_strset		env_keys;
};

void			sh_context_init(t_sh_context *dst);

# define SH_ENV_SIZE(C)		((C).env_keys.count)

/*
** Set a variable's value
*/
void			sh_var_set(t_sh_context *c, t_sub key, t_sub value);

/*
** Unset a variable
** Do nothing if the variable is not already set
*/
void			sh_var_unset(t_sh_context *c, t_sub key);

/*
** Get a variable value by key
** If the variable is unset, return an empty sub
*/
t_sub			sh_var_get(t_sh_context const *c, t_sub key);

/*
** Build the environ array
** 'env' array must be of size SH_ENV_SIZE(c) + 1
** A NULL value is put at the end of the array
*/
void			sh_env_build(t_sh_context const *c, char const **env);

/*
** Set/unset exported state of a variable
*/
void			sh_env_export(t_sh_context *c, t_sub key, bool exported);

#endif
