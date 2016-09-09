/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/09 18:28:32 by juloo             #+#    #+#             */
/*   Updated: 2016/09/09 18:41:04 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "ft/libft.h"

/*
** ========================================================================== **
** Some utils
*/

/*
** Copy content of 'S' into buffer 'DST'
** and return SUB(DST, S.length)
*/
# define SUB_DST(DST, S)	SUB(ft_memcpy(DST, (S).str, (S).length), (S).length)

/*
** Test if all chars in 'sub' match 'is'
*/
bool			ft_subis(t_sub sub, t_is is);

/*
** Test if 'sub' is a valid identifier
** [a-zA-Z_][a-zA-Z0-9_]*
*/
bool			ft_subis_identifier(t_sub sub);

#endif
