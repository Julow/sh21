/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_push.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/25 18:24:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 17:30:19 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			*ft_spanlist_push(t_spanlist *spanlist, uint32_t length,
					int32_t priority)
{
	t_spanlist_span		*span;
	t_spanlist_span		*last;

	last = spanlist->spans.last;
	while (last != NULL && last->priority != priority)
		last = LIST_PREV(last);
	span = ft_listadd(&spanlist->spans, last, spanlist->data_size);
	span->from = (last == NULL) ? 0 : last->to;
	span->to = span->from + length;
	span->priority = priority;
	return (ENDOF(span));
}
