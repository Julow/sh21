/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spantree.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 15:52:55 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/20 18:57:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPANTREE_H
# define SPANTREE_H

# include "ft/libft.h"

typedef struct s_spantree		t_spantree;
typedef struct s_spantree_node	t_spantree_node;
typedef struct s_spantree_data	t_spantree_data;

/*
** ========================================================================== **
** Span tree
*/

struct			s_spantree_node
{
	int32_t			from;
	int32_t			to;
	void			*data;
	t_spantree_node	*childs;
	t_spantree_node	*next;
};

struct			s_spantree
{
	t_spantree_node	*spans;
	t_spantree_node	*unused;
	uint32_t		unused_count;
};

struct			s_spantree_data
{
	void			*data;
	t_spantree_data	*prev;
};

/*
** Add a span
*/
void			ft_spantree_add(t_spantree *tree, t_vec2i range, void *data);

/*
** 'callback' is of type:
**  void (*)(void *self, int32_t from, int32_t to, t_spantree_data *data)
*/
void			ft_spantree_iter(t_spantree const *tree, t_vec2i range,
					t_callback callback);

/*
** Return span the span at 'index'
*/
t_spantree_node	*ft_spantree_at(t_spantree *tree, int32_t index,
					uint32_t depth);

/*
** NOT IMPLEMENTED
** remove span when callback return true
** 'callback' is of type:
**  bool (*)(void *self, int32_t from, int32_t to, t_spantree_data *data)
*/
void			ft_spantree_clear(t_spantree *tree, t_vec2i range,
					t_callback callback);
#endif
