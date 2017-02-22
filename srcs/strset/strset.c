/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strset.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 13:30:28 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/22 14:30:59 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/strset.h"

t_strset_node	*ft_strset_node(t_sub str)
{
	t_strset_node	*node;
	char			*dst;

	node = MALLOC(sizeof(t_strset_node) + str.length + 1);
	dst = ENDOF(node);
	memcpy(dst, str.str, str.length);
	dst[str.length] = '\0';
	*node = STRSET_NODE(SUB(dst, str.length));
	return (node);
}

int				strset_cmp(t_strset_node const *node, t_sub const *key)
{
	if (node->value.length != key->length)
		return (node->value.length - key->length);
	return (memcmp(node->value.str, key->str, key->length));
}
