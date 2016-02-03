/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_color_loader.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:43:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/03 19:45:14 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_COLOR_LOADER_H
# define SYNTAX_COLOR_LOADER_H

# include "ft/libft.h"
# include "ft/parser.h"

/*
** ========================================================================== **
** Syntax color loader
*/

/*
** Load a syntax color
** -
** Return NULL if it does not exist
*/
t_parser const	*load_syntax_color(t_sub name);

#endif
