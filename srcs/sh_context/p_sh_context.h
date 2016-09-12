/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_context.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 15:11:57 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/12 19:47:47 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_CONTEXT_H
# define P_SH_CONTEXT_H

# include "ft/libft.h"
# include "sh/context.h"

/*
** ========================================================================== **
** Sh context private
*/

# define SH_C_VAR(LEN,KLEN)	((t_sh_c_var){SET_HEAD(), (LEN), (KLEN)})

int				sh_c_var_cmp(t_sh_c_var const *v, t_sub const *key);
int				sh_c_builtin_cmp(t_sh_c_builtin const *v, t_sub const *key);
int				sh_c_function_cmp(t_sh_c_function const *v, t_sub const *key);
int				sh_c_path_cmp(t_sh_c_path const *v, t_sub const *key);

#endif
