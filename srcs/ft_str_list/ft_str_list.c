/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 12:58:12 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/06 18:03:30 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/str_list.h"

void			ft_str_list_append(t_str_list *lst, t_sub str)
{
	uint32_t		align;

	if (lst->last_break)
	{
		if (lst->buff.length > 0)
		{
			DSTR_APPEND(&lst->buff, '\0');
			align = STR_LIST_NEXT(*lst, lst->last_offset) + lst->last_offset;
			DSTR_EXTEND(&lst->buff, align - lst->buff.length);
			lst->buff.length = align;
		}
		DSTR_EXTEND(&lst->buff, sizeof(uint32_t));
		lst->last_offset = lst->buff.length;
		*(uint32_t*)(lst->buff.str + lst->buff.length) = 0;
		lst->buff.length += sizeof(uint32_t);
		lst->last_break = false;
		lst->count++;
	}
	*(uint32_t*)(lst->buff.str + lst->last_offset) += str.length;
	ft_dstradd(&lst->buff, str);
}

void			ft_str_list_break(t_str_list *lst)
{
	lst->last_break = true;
}

void			ft_str_list_clear(t_str_list *lst)
{
	ft_dstrclear(&lst->buff);
	*lst = STR_LIST();
}
