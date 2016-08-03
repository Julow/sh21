/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:09:25 by juloo             #+#    #+#             */
/*   Updated: 2016/08/03 23:45:03 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ft/ft_in.h"
# include "ft/libft.h"
# include "sh/ast.h"

typedef enum e_sh_parse_err_t	t_sh_parse_err_t;
typedef struct s_sh_parse_err	t_sh_parse_err;

/*
** ========================================================================== **
** Sh parser
*/

enum			e_sh_parse_err_t
{
	SH_E_UNEXPECTED_ERROR,
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

/*
** SH_PARSE_ERR()		sh_parse_err constructor
** D_SH_PARSE_ERR(ERR)	sh_parse_err destructor
*/
# define SH_PARSE_ERR()		((t_sh_parse_err){SH_E_UNEXPECTED_ERROR, DSTR0()})
# define D_SH_PARSE_ERR(E)	(ft_dstrclear(&((E).str)))

/*
** Parse a full line + heredoc
** -
** On success return true and fill 'dst' with the parsed command
** On error return false and fill 'error' with the corresponding error
** -
** If 'err' is NULL, no error is reported
*/
bool			sh_parse_line(t_in *in, t_sh_compound *dst, t_sh_parse_err *err);

#endif
