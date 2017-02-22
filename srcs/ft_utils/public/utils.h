/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/09 18:28:32 by juloo             #+#    #+#             */
/*   Updated: 2017/02/22 14:32:17 by jaguillo         ###   ########.fr       */
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
# define SUB_DST(DST, S)	SUB(memcpy(DST, (S).str, (S).length), (S).length)

/*
** Test if all chars in 'sub' match 'mask'
*/
bool			ft_subis(t_sub sub, uint32_t mask);

/*
** Test if 'sub' is a valid identifier
** [a-zA-Z_][a-zA-Z0-9_]*
*/
bool			ft_subis_identifier(t_sub sub);

#endif
