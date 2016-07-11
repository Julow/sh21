/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strset.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 13:30:05 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/11 14:14:43 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRSET_H
# define STRSET_H

# include "ft/libft.h"
# include "ft/set.h"

typedef t_set					t_strset;
typedef struct s_strset_node	t_strset_node;

/*
** ========================================================================== **
** String set
*/

/*
** Construct a string set
*/
# define STRSET(F)			(SET(&strset_cmp, (F)))

/*
** A string set node object
** set_h			=> set header (used by ft::set functions)
** value			=> string value
** -
** STRSET_NODE(STR)		Construct a strset_node with value 'STR'
** STRSET_CSTR(NODE)	Return the 0-terminated string of a node
** 							(Only on node created with ft_strset_node)
*/
struct			s_strset_node
{
	t_set_h			set_h;
	t_sub			value;
};

# define STRSET_NODE(STR)	((t_strset_node){SET_HEAD(), (STR)})

# define STRSET_CSTR(NODE)	(ENDOF((t_strset_node const*)(NODE)))

/*
** Create a new string set node
** Copy 'str' value
*/
t_strset_node	*ft_strset_node(t_sub str);

/*
** -
*/

int				strset_cmp(t_strset_node const *node, t_sub const *key);

#endif
