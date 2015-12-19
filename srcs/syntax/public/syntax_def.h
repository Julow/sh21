/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_def.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 21:02:47 by juloo             #+#    #+#             */
/*   Updated: 2015/12/19 15:07:25 by juloo            ###   ########.fr       */
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
	t_sub			end;
	t_sub			inherit;
	t_vector		tokens;
	// t_syntax_def_t	*match;
};

struct		s_syntax_def_t
{
	t_sub			token;
	t_sub			scope;
	t_sub			syntax;
};

# define SYNTAX_DEF_T(...)			VECTORC(((t_syntax_def_t[]){__VA_ARGS__}))
# define SYNTAX_T(N,S,...)			{SUBC(N), SUBC(S), SUBC("" __VA_ARGS__)}
# define SYNTAX_DEF(NAME, END, ...)	{SUBC(NAME), .end = SUBC(END), __VA_ARGS__}

bool		build_syntax(t_hmap *dst, t_vector const *syntaxes);

#endif
