/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 10:25:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/18 11:01:20 by jaguillo         ###   ########.fr       */
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
# include "sh/parser_context.h"

typedef struct s_sh_context		t_sh_context;
typedef int						(*t_sh_builtin)(t_sh_context *c,
									void *data, t_argv args);

typedef struct s_sh_c_var			t_sh_c_var;
typedef struct s_sh_c_builtin		t_sh_c_builtin;
typedef struct s_sh_c_function		t_sh_c_function;
typedef struct s_sh_c_path			t_sh_c_path;

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
** path_cache		=> path lookup cache
** parser_context	=> parser config (alias, ...)
*/
struct			s_sh_context
{
	t_set				vars;
	t_strset			env_keys;
	t_str_list			pos_params;
	uint32_t			last_status;
	t_set				builtins;
	t_set				functions;
	t_set				path_cache;
	t_sh_parser_context	parser_context;
};

void			sh_context_init(t_sh_context *dst,
					char const *const *argv, char const *const *env);

# define SH_C_ENV_SIZE(C)		((C).env_keys.count + 1)

/*
** ========================================================================== **
** Variables
*/

/*
** Var are stored as 0-terminated string
** length			=> total length of the variable string
** key_len			=> length of the key
*/
struct			s_sh_c_var
{
	t_set_h			set_h;
	uint32_t		length;
	uint32_t		key_len;
};

# define SH_C_VAR_STR(V)	(ENDOF(V))

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
** ENDOF()			=> builtin data
*/
struct			s_sh_c_builtin
{
	t_set_h			set_h;
	t_sub			name;
	t_sh_builtin	f;
};

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

struct			s_sh_c_function
{
	t_set_h			set_h;
	t_sub			name;
	t_sh_cmd		body;
};

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

struct			s_sh_c_path
{
	t_set_h			set_h;
	t_sub			name;
	t_sub			path;
};

/*
** Look for executable file 'name' in ':'-separated path of '$PATH'
** Put result to 'dst'
** -
** Cache result for faster lookup
** -
** If 'name' contains any '/', it is used as result
*/
bool			sh_c_path_search(t_sh_context *c, t_sub name, t_dstr *dst);

/*
** Clear path cache
*/
void			sh_c_path_clear(t_sh_context *c);

/*
** Set path for a command
*/
void			sh_c_path_set(t_sh_context *c, t_sub name, t_sub path);

/*
** Get saved path for a command
** Return an empty sub if not found
*/
t_sub			sh_c_path_get(t_sh_context const *c, t_sub name);

/*
** Remove the saved path for a command
** Return false if not found
*/
bool			sh_c_path_remove(t_sh_context *c, t_sub name);

#endif
