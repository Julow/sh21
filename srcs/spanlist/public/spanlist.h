/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spanlist.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/25 12:20:00 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/25 18:29:43 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPANLIST_H
# define SPANLIST_H

# include "ft/ft_list.h"
# include "ft/libft.h"

typedef struct s_spanlist		t_spanlist;
typedef struct e_spanlist_span	t_spanlist_span;

/*
** ========================================================================== **
** Span list
*/

struct			s_spanlist_span
{
	uint32_t		from;
	uint32_t		to;
	int32_t			priority;
};

struct			s_spanlist
{
	t_list			spans;
	uint32_t const	data_size;
};

# define SPANLIST(T)		((t_spanlist){LIST(t_spanlist_span), sizeof(T)})

/*
** Add a span to the end
*/
void			*ft_spanlist_push(t_spanlist *spanlist, uint32_t length,
					int32_t priority);

/*
** Call 'callback' for each span
** 'callback' is of type:
**  void (*)(void *env, t_vec2u span, void const *span_data)
*/
void			ft_spanlist_iter(t_spanlist const *spanlist,
					t_callback callback, t_vec2u range);

/*
** Set a span
** NOT IMPLEMENTED
*/
// void			ft_spanlist_set(t_spanlist *spanlist, t_vec2u range,
					// t_spanlist_type type, uint32_t value, int32_t priority);

/*
** Clear by priority
** NOT IMPLEMENTED
*/
void			ft_spanlist_clear(int32_t priority);

#endif
