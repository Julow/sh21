/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_def.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 21:02:47 by juloo             #+#    #+#             */
/*   Updated: 2016/01/08 16:58:21 by jaguillo         ###   ########.fr       */
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
	t_sub			scope;
	t_sub			inherit;
	t_vector		tokens;
	t_vector		match;
};

struct		s_syntax_def_t
{
	t_sub			token;
	t_sub			scope;
	char const		*syntax;
	bool			end;
};

# define SYNTAX_DEF_T(...)			VECTORC(((t_syntax_def_t[]){__VA_ARGS__}))
# define SYNTAX_T(N,S,...)			{.token=SUBC(N),SUBC(S),##__VA_ARGS__}
# define SYNTAX_DEF(N,S,...)		{SUBC(N),SUBC(S),__VA_ARGS__}

bool		build_syntax(t_hmap *dst, t_vector const *syntaxes);

#endif
