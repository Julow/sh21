/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_sh_context.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 15:11:57 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 15:20:37 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_SH_CONTEXT_H
# define P_SH_CONTEXT_H

typedef struct s_sh_var			t_sh_var;

/*
** ========================================================================== **
** Sh context private
*/

# define SH_CONTEXT()		((t_sh_context){SET(sh_var_cmp, 0), STRSET(0)})

/*
** Var are stored as 0-terminated string
** length			=> total length of the variable string
** key_len			=> length of the key
*/
struct			s_sh_var
{
	t_set_h			set_h;
	uint32_t		length;
	uint32_t		key_len;
};

# define SH_VAR_STR(V)		(ENDOF(V))
# define SH_VAR(LEN,KLEN)	((t_sh_var){SET_HEAD(), (LEN), (KLEN)})

int				sh_var_cmp(t_sh_var const *v, t_sub const *key);

#endif
