/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_mod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 18:56:36 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/15 19:11:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_bindings.h"

bool			editor_bind_extra_mod(t_editor *editor, uint32_t flags)
{
	if ((editor->extra_mods & flags) == flags)
		return (false);
	editor->extra_mods |= flags;
	return (true);
}
