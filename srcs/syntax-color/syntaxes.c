/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/09 15:18:40 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/09 15:29:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

extern t_syntax_color_def const	g_syntax_color_utils;
extern t_syntax_color_def const	g_syntax_color_sh;
extern t_syntax_color_def const	g_syntax_color_xml;

t_vector const	g_syntaxes_color_def = VECTOR(t_syntax_color_def const*,
	&g_syntax_color_utils,
	&g_syntax_color_sh,
	&g_syntax_color_xml,
);
