/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_iter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/25 18:33:20 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/25 18:37:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			ft_spanlist_iter(t_spanlist const *spanlist,
					t_callback callback, t_vec2u range)
{
	t_spanlist_span	*span;
	t_spanlist_span	*tmp;

	span = LIST_BEGIN(&spanlist->spans);
	while ((span = LIST_NEXT(span)))
	{
		tmp = LIST_NEXT(span);
		if (tmp != NULL && tmp->from < span->to
			&& tmp->priority > span->priority)
		{
			if (tmp->from > span->from)
				CALL(void, callback, VEC2U(span->from, tmp->from), ENDOF(span));
		}
		// else if span->priority > tmp->priority
		else
			CALL(void, callback, VEC2U(span->from, span->to), ENDOF(span));
	}
}
