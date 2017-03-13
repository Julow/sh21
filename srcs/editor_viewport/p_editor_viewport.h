/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_editor_viewport.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/12 02:05:13 by juloo             #+#    #+#             */
/*   Updated: 2017/03/12 02:06:23 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_EDITOR_VIEWPORT_H
# define P_EDITOR_VIEWPORT_H

# include "editor_viewport.h"

/*
** ========================================================================== **
*/

/*
** Update the line heights
** If 'event' is false, do not trigger events
*/
void			editor_update_line_heights(t_editor_viewport *v, bool event);

#endif
