/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:18:44 by juloo             #+#    #+#             */
/*   Updated: 2016/02/09 15:23:24 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "ft/ft_vector.h"
# include "ft/libft.h"
# include "ft/parser.h"
# include "ft/parser_def.h"

# include "syntax_color.h"

typedef struct s_syntax_color_def	t_syntax_color_def;

/*
** ========================================================================== **
** Def
*/

struct			s_syntax_color_def
{
	t_sub			name;
	t_sub			main_parser;
	t_vector		def;
	t_vector		depend;
};

# define SYNTAX_COLOR(N,M,...)		{SUBC(N),SUBC(M),##__VA_ARGS__}
# define SYNTAX_COLOR_DEPEND(...)	.depend=VECTORC(((t_sub[]){__VA_ARGS__}))

#endif
