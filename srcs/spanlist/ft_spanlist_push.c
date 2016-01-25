/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_push.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/25 18:24:59 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/25 18:30:52 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			*ft_spanlist_push(t_spanlist *spanlist, uint32_t length,
					int32_t priority)
{
	t_spanlist_span					*span;
	t_spanlist_span const *const	last = spanlist->spans.last;

	span = ft_listadd(&spanlist->spans, last, spanlist->data_size);
	span->from = (last == NULL) ? 0 : last->to;
	span->to = span->from + length;
	span->priority = priority;
	return (ENDOF(span));
}
