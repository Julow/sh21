/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_iter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/25 18:33:20 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 12:46:38 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			ft_spanlist_iter(t_spanlist const *spanlist,
					t_callback callback, t_vec2u range)
{
	t_spanlist_span	*span;
	t_spanlist_span	*next;
	t_vec2u			span_range;

	span = LIST_NEXT(LIST_IT(&spanlist->spans));
	while (span != NULL && span->to <= range.x)
	{
		ft_printf("skip%n");
		span = LIST_NEXT(span);
	}
	while (span != NULL)
	{
		next = LIST_NEXT(span);
		while (next != NULL && next->priority < span->priority
				&& next->from < span->to)
			next = LIST_NEXT(next);
		if (next != NULL && next->from < span->to)
			span_range.y = MIN(next->from, range.y);
		else
			span_range.y = MIN(span->to, range.y);
		span_range.x = MAX(range.x, span->from);
		if (span_range.x < span_range.y)
			CALL(void, callback, span_range, ENDOF(span));
		if (span_range.y == range.y)
			break ;
		span = next;
	}
}
