/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_def.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 21:02:47 by juloo             #+#    #+#             */
/*   Updated: 2016/01/19 15:18:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_DEF_H
# define SYNTAX_DEF_H

# include "ft/ft_hmap.h"
# include "ft/ft_vector.h"
# include "ft/libft.h"

typedef struct s_syntax_def		t_syntax_def;
typedef struct s_syntax_def_t	t_syntax_def_t;

/*
** ========================================================================== **
** Syntax def
** -
** Struct used to declare a syntax
*/

struct		s_syntax_def
{
	t_sub			name;
	void			*data;
	t_sub			inherit;
	t_vector		tokens;
	t_vector		match;
};

struct		s_syntax_def_t
{
	t_sub			token;
	void			*data;
	char const		*syntax;
	bool			end;
};

# define SYNTAX_DEF_T(...)			VECTORC(((t_syntax_def_t[]){__VA_ARGS__}))
# define SYNTAX_T(N,DATA,...)		{.token=SUBC(N),DATA,##__VA_ARGS__}
# define SYNTAX_DEF(N,DATA,...)		{SUBC(N),DATA,__VA_ARGS__}

bool		build_syntax(t_hmap *dst, t_vector const *syntaxes);

#endif
