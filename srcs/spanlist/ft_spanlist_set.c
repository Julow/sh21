/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_set.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 13:52:09 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 15:41:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			*ft_spanlist_set(t_spanlist *spanlist, t_vec2u range,
					uint32_t priority)
{
	t_spanlist_span	*span;

	span = LIST_IT(&spanlist->spans);
	while ((span = LIST_NEXT(span)) && span->from < range.x)
		;
	if (span != NULL)
		span = LIST_PREV(span);
	span = ft_listadd(&spanlist->spans, span, spanlist->data_size);
	span->from = range.x;
	span->to = range.y;
	span->priority = priority;
	return (ENDOF(span));
}
