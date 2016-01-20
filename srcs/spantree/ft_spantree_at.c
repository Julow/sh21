/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spantree_at.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 17:03:29 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/20 19:29:04 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spantree.h"

t_spantree_node	*spantree_at(t_spantree_node *span, int32_t index,
							t_spantree_data *prev, uint32_t depth)
{
	t_spantree_data			data;

	for (; span != NULL; span = span->next)
	{
		if (index >= span->to)
			continue ;
		if (index < span->from)
			break ;
		data = (t_spantree_data){span, prev};
		if ((span = spantree_at(span->childs, index, &data, depth)) != NULL)
			return (span);
		prev = &data;
		break ;
	}
	if (prev == NULL)
		return (NULL);
	while (depth-- > 0 && prev->prev != NULL)
		prev = prev->prev;
	return (prev->data);
}

t_spantree_node	*ft_spantree_at(t_spantree *tree, int32_t index, uint32_t depth)
{
	return (spantree_at(tree->spans, index, NULL, depth));
}
