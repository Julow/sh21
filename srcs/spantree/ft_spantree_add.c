/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spantree_add.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 15:58:49 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/20 16:15:20 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spantree.h"

static t_spantree_node	*spantree_new_node(t_spantree *tree, t_vec2i range,
							void *data, t_spantree_node *next)
{
	t_spantree_node	*span;

	if ((span = tree->unused) != NULL)
	{
		tree->unused = span->next;
		tree->unused_count--;
	}
	else
	{
		span = NEW(t_spantree_node);
	}
	*span = (t_spantree_node){range.x, range.y, data, NULL, next};
	return (span);
}

static void		spantree_add(t_spantree *tree, t_spantree_node **span,
					t_vec2i range, void *data)
{
	int32_t			tmp;

	if (*span == NULL || (*span)->from >= range.y)
	{
		*span = spantree_new_node(tree, range, data, *span);
		return ;
	}
	while (true)
	{
		if ((*span)->from > range.x)
		{
			tmp = (*span)->from;
			*span = spantree_new_node(tree, VEC2I(range.x, tmp), data, *span);
		}
		else if ((*span)->to > range.x)
		{
			tmp = MIN((*span)->to, range.y);
			spantree_add(tree, &(*span)->childs, VEC2I(range.x, tmp), data);
		}
		range.x = tmp;
		if (range.x >= range.y || (*span)->from >= range.y)
			break ;
		if ((*span)->next == NULL)
		{
			if (range.x < range.y)
				(*span)->next = spantree_new_node(tree, range, data, NULL);
			break ;
		}
		span = &(*span)->next;
	}
}

void			ft_spantree_add(t_spantree *tree, t_vec2i range, void *data)
{
	if (range.x < range.y)
		spantree_add(tree, &tree->spans, range, data);
}
