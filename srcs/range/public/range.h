/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   range.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/14 14:45:49 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/14 15:45:00 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANGE_H
# define RANGE_H

# include "ft/libft.h"

typedef struct s_range		t_range;

/*
** ========================================================================== **
** Range
*/

struct		s_range
{
	int32_t		from;
	int32_t		to;
};

/*
** Init a range FROM -> TO
** Each param are used once
*/
# define RANGE(FROM, TO)	((t_range){(FROM), (TO)})

/*
** Init a range MIN(A, B) -> MAX(A, B)
*/
# define SRANGE(A, B)		(((A) > (B)) ? RANGE(B, A) : RANGE(A, B))

/*
** Init a range MIN(A, B) -> MAX(A, B)
** Like SRANGE
*/
t_range		ft_range(int32_t from, int32_t to);

/*
** Clip range into 'clip'
** Also check if from < to and swap if needed
*/
t_range		ft_rangeclip(t_range range, t_range clip);

/*
** Return the range of char pointed by 'b' into 'a'
*/
t_range		ft_subrange(t_sub a, t_sub b);

#endif
