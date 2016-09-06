/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 12:57:25 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/06 17:55:52 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_LIST_H
# define STR_LIST_H

# include "ft/ft_dstr.h"
# include "ft/libft.h"

typedef struct s_str_list		t_str_list;

/*
** ========================================================================== **
** String list
** List of string dynamically allocated
** -
** String are stored in a buffer like this:
** <length><string><\0><padding>
** length			=> the length of the string in byte
** string			=> string of '<length>' characters
** \0				=> put a terminating '\0' at end of each strings
** padding			=> the next string is at += ALIGN_UP(<length>, 4)
*/

struct			s_str_list
{
	t_dstr			buff;
	bool			last_break:1;
	uint32_t		last_offset:31;
	uint32_t		count;
};

# define STR_LIST()			((t_str_list){DSTR0(), true, 0, 0})

/*
** Return the offset between the string at 'OFF' and the next
*/
# define STR_LIST_NEXT(L, OFF)	(ALIGN_UP(STR_LIST_LEN(L, OFF) + 1, 4) + sizeof(uint32_t))

/*
** Return the sub/str/length of the string at 'OFF'
*/
# define STR_LIST_STR(L, OFF)	((L).buff.str + (OFF) + sizeof(uint32_t))
# define STR_LIST_SUB(L, OFF)	(SUB(STR_LIST_STR(L, OFF), STR_LIST_LEN(L, OFF)))
# define STR_LIST_LEN(L, OFF)	(*(uint32_t const*)((L).buff.str + (OFF)))

/*
** Return the end offset
*/
# define STR_LIST_END(L)		((L).buff.length)

/*
** Append 'str' to the last string of the list
*/
void			ft_str_list_append(t_str_list *lst, t_sub str);

/*
** Validate the last string
** Next call to ft_str_list_append will add a new list
*/
void			ft_str_list_break(t_str_list *lst);

/*
** Clear
** The string list is reusable after a clear
*/
void			ft_str_list_clear(t_str_list *lst);

#endif
