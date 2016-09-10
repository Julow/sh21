/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_context.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 15:11:57 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/10 11:13:54 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_CONTEXT_H
# define P_SH_CONTEXT_H

# include "ft/libft.h"
# include "ft/set.h"
# include "sh/context.h"

typedef struct s_sh_c_var			t_sh_c_var;
typedef struct s_sh_c_builtin		t_sh_c_builtin;
typedef struct s_sh_c_function		t_sh_c_function;

/*
** ========================================================================== **
** Sh context private
*/

/*
** ========================================================================== **
** vars
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
# define SH_C_VAR(LEN,KLEN)	((t_sh_c_var){SET_HEAD(), (LEN), (KLEN)})

int				sh_c_var_cmp(t_sh_c_var const *v, t_sub const *key);

/*
** ========================================================================== **
** builtins
*/

/*
** name				=> builtin name
** f				=> builtin function
** ENDOF()			=> builtin data
*/
struct			s_sh_c_builtin
{
	t_set_h			set_h;
	t_sub			name;
	t_sh_builtin	f;
};

int				sh_c_builtin_cmp(t_sh_c_builtin const *v, t_sub const *key);

/*
** ========================================================================== **
** functions
*/

struct			s_sh_c_function
{
	t_set_h			set_h;
	t_sub			name;
	t_sh_cmd		body;
};

int				sh_c_function_cmp(t_sh_c_function const *v, t_sub const *key);

#endif
