/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_context.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/18 10:44:45 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/18 12:31:27 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CONTEXT_H
# define PARSER_CONTEXT_H

# include "ft/libft.h"
# include "ft/set.h"

typedef struct s_sh_parser_context	t_sh_parser_context;
typedef struct s_sh_c_alias			t_sh_c_alias;

/*
** ========================================================================== **
** Parser context
** -
** Currently only store aliases
*/

struct			s_sh_parser_context
{
	t_set			aliases;
};

# define SH_PARSER_CONTEXT()	((t_sh_parser_context){SET(&sh_c_alias_cmp, 0)})

/*
** Alias
*/

struct			s_sh_c_alias
{
	t_set_h			set_h;
	t_sub			name;
	t_sub			val;
};

# define SH_ALIAS_NAME_FORBIDDEN	"$`\\ \t\"'#|&;()><="

/*
** Set an alias
** If an alias with the same name is already set, override it
*/
void			sh_c_alias_set(t_sh_parser_context *c, t_sub name, t_sub val);

/*
** Return an alias's value
** Return NULL if not found
*/
t_sub const		*sh_c_alias_get(t_sh_parser_context const *c, t_sub name);

/*
** Unset an alias
** Return true on success, false if not found
*/
bool			sh_c_alias_unset(t_sh_parser_context *c, t_sub name);

/*
** -
*/

int				sh_c_alias_cmp(t_sh_c_alias const *v, t_sub const *key);

#endif
