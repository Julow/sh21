/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_color.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/03 19:43:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/09 23:52:53 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_COLOR_LOADER_H
# define SYNTAX_COLOR_LOADER_H

# include "ft/libft.h"

typedef void					t_syntax_color;
typedef struct s_color_scheme	t_color_scheme;

/*
** ========================================================================== **
** Exec syntax
*/

struct			s_color_scheme
{
	t_sub			scope;
	void const		*data;
};

/*
** 'callback' is of type:
**   void (*)(void *self, t_vec2u range, void const *scheme_data);
** Notes: ranges are always continuous and scheme_data can be NULL
** 'syntax' is a ptr returned by a previous call to load_syntax_color
** 'color_scheme' is a vector of t_color_scheme
*/
void			exec_syntax_color(t_in *in, t_syntax_color const *syntax,
					t_vector const *color_scheme, t_callback callback);

/*
** ========================================================================== **
** Syntax loader
*/

/*
** Load a syntax color
** -
** Return NULL if it does not exist
*/
t_syntax_color const	*load_syntax_color(t_sub name);

#endif
