/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 12:27:34 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/09 18:13:25 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH__PARSER_H
# define SH__PARSER_H

# include "ft/ft_in.h"
# include "ft/libft.h"
# include "sh/cmd.h"

typedef enum e_sh_parse_err_t	t_sh_parse_err_t;
typedef struct s_sh_parse_err	t_sh_parse_err;

/*
** ========================================================================== **
** Sh parser
*/

enum			e_sh_parse_err_t
{
	SH_E_UNEXPECTED_TOKEN,
	SH_E_UNEXPECTED_EOF,
	SH_E_UNCLOSED_STRING,
	SH_E_UNCLOSED_SUBSHELL,
};

struct			s_sh_parse_err
{
	t_sh_parse_err_t	err;
	t_dstr				str;
};

# define SH_PARSE_ERR()		((t_sh_parse_err){0, DSTR0()})

/*
** Parse a single command
** On error, return NULL and set 'err' to the corresponding error
** On success, return a t_sh_cmd object
*/
t_sh_cmd		*sh_parse_cmd(t_in *in, t_sh_parse_err *err);

/*
** Parse a pipeline of commands
*/
// t_sh_cmd		*sh_parse_pipeline(t_in *in, t_sh_parse_err *err);

/*
** Parse a compound command (list of pipeline separated by '&&' or '||')
*/
t_sh_cmd		*sh_parse_compound(t_in *in, t_sh_parse_err *err);

# define D_SH_PARSE_ERR(ERR)	(ft_dstrclear(&(ERR).str))

#endif
