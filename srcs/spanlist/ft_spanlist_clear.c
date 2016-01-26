/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spanlist_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 11:57:31 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/26 12:02:33 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/spanlist.h"

void			ft_spanlist_clear(t_spanlist *spanlist, int32_t priority)
{
	t_spanlist_span	*span;

	span = LIST_IT(spanlist);
	while ((span = LIST_NEXT(span)))
		if (span->priority == priority)
			span = ft_listremove(&spanlist->spans, span);
}
