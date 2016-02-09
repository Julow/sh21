/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_color.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:43:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/09 15:23:02 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_COLOR_LOADER_H
# define SYNTAX_COLOR_LOADER_H

# include "ft/libft.h"

typedef void const		t_syntax_color;

/*
** ========================================================================== **
** Syntax loader
*/

/*
** Load a syntax color
** -
** Return NULL if it does not exist
*/
t_syntax_color	*load_syntax_color(t_sub name);

#endif
