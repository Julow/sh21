/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_mod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 18:56:36 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/04 12:19:57 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"

bool			editor_bind_extra_mod(void *data, t_editor *editor)
{
	uintptr_t const	flags = (uintptr_t)data;

	if ((editor->extra_mods & flags) == flags)
		return (false);
	editor->extra_mods |= flags;
	return (true);
}
