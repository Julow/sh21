/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 10:25:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/11 19:29:03 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "ft/argv.h"
# include "ft/libft.h"
# include "ft/set.h"
# include "ft/str_list.h"
# include "ft/strset.h"
# include "sh/ast.h"

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
** functions		=> user defined functions
*/
struct			s_sh_context
{
	t_set			vars;
	t_strset		env_keys;
	t_str_list		pos_params;
	uint32_t		last_status;
	t_set			builtins;
	t_set			functions;
};

void			sh_context_init(t_sh_context *dst,
					char const *const *argv, char const *const *env);

# define SH_C_ENV_SIZE(C)		((C).env_keys.count + 1)

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
** The returned sub '.str' is always '\0'-terminated
*/
t_sub			sh_c_var_get(t_sh_context const *c, t_sub key);

/*
** Build the environ array
** 'env' array must be of size SH_C_ENV_SIZE(c)
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

/*
** ========================================================================== **
** Functions
*/

/*
** Search for a function
** Return it's body if found, NULL otherwise
*/
t_sh_cmd const	*sh_c_function_get(t_sh_context const *c, t_sub name);

/*
** Define a function
** If a function with the same name already exists, override it
** 'f' is copied
*/
void			sh_c_function_define(t_sh_context *c, t_sh_func_def const *f);

/*
** ========================================================================== **
** Path search
*/

/*
** Look for executable file 'name' in ':'-separated path of 'path'
** Put result to 'dst'
** -
** If 'name' contains any '/', it is used as result
** TODO: cache binary locations
*/
bool			sh_c_path_search(t_sh_context *c, t_sub path,
					t_sub name, t_dstr *dst);

#endif
