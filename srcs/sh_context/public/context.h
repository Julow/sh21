/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 10:25:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 16:53:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "ft/argv.h"
# include "ft/libft.h"
# include "ft/set.h"
# include "ft/str_list.h"
# include "ft/strset.h"

typedef struct s_sh_context		t_sh_context;
typedef int						(*t_sh_builtin)(t_sh_context *c,
									void *data, t_argv args);

/*
** ========================================================================== **
** Sh context
*/

/*
** Context in which commands are executed
** vars				=> variables
** env_keys			=> key of variables in env
** pos_params		=> positional params
** last_status		=> $?
** builtins			=> builtin set
*/
struct			s_sh_context
{
	t_set			vars;
	t_strset		env_keys;
	t_str_list		pos_params;
	uint32_t		last_status;
	t_set			builtins;
};

void			sh_context_init(t_sh_context *dst);

# define SH_C_ENV_SIZE(C)		((C).env_keys.count)

/*
** ========================================================================== **
** Variables
*/

/*
** Set a variable's value
*/
void			sh_c_var_set(t_sh_context *c, t_sub key, t_sub value);

/*
** Unset a variable
** Do nothing if the variable is not already set
*/
void			sh_c_var_unset(t_sh_context *c, t_sub key);

/*
** Get a variable value by key
** If the variable is unset, return an empty sub
*/
t_sub			sh_c_var_get(t_sh_context const *c, t_sub key);

/*
** Build the environ array
** 'env' array must be of size SH_C_ENV_SIZE(c) + 1
** A NULL value is put at the end of the array
*/
void			sh_c_env_build(t_sh_context const *c, char const **env);

/*
** Set/unset exported state of a variable
*/
void			sh_c_env_export(t_sh_context *c, t_sub key, bool exported);

/*
** ========================================================================== **
** Builtins
*/

/*
** Search a builtin by it's name
** If true, fill 'builtin' and 'builtin_data' and return true
** otherwise return false
*/
bool			sh_c_builtin_get(t_sh_context const *c, t_sub name,
					t_sh_builtin *builtin, void **builtin_data);

/*
** Register a builtin
** Return a ptr the builtin's data of 'data_size' size on success
** Return NULL if a builtin with the same name is already registered
*/
void			*sh_c_builtin_register(t_sh_context *c, t_sub name,
					t_sh_builtin builtin_f, uint32_t data_size);

/*
** Unregister a builtin
** 'clean' (if not NULL) is called to clean the builtin's data
** Do nothing if the builtin was not registered
*/
void			sh_c_builtin_unregister(t_sh_context *c, t_sub name,
					void (*clean)(void *data));

#endif
