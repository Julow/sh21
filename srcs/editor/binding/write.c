/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 13:52:07 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 14:00:02 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_internal.h"

bool		editor_bind_write(char const *str, t_editor *editor)
{
	t_sub const	sub = ft_sub(str, 0, -1);

	editor_write(editor,
		VEC2U(editor->cursor, editor->cursor + editor->sel), sub);
	editor_set_cursor(editor, editor->cursor + sub.length, 0);
	return (true);
}
