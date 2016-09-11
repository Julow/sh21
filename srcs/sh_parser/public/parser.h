/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:09:25 by juloo             #+#    #+#             */
/*   Updated: 2016/09/11 15:53:45 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ft/ft_in.h"
# include "ft/libft.h"
# include "sh/ast.h"

typedef enum e_sh_parse_err_t				t_sh_parse_err_t;
typedef enum e_sh_parse_err_unterminated	t_sh_parse_err_unterminated;
typedef struct s_sh_parse_err				t_sh_parse_err;

/*
** ========================================================================== **
** Sh parser
*/

enum			e_sh_parse_err_t
{
	SH_E_ERROR,
	SH_E_UNEXPECTED,
	SH_E_EOF,
	SH_E_INVALID,
	SH_E_UNTERMINATED,
};

enum			e_sh_parse_err_unterminated
{
	SH_E_UNTERMINATED_AND,
	SH_E_UNTERMINATED_OR,
	SH_E_UNTERMINATED_PIPE,
	SH_E_UNTERMINATED_SUBSHELL,
	SH_E_UNTERMINATED_SUBST_SUBSHELL,
	SH_E_UNTERMINATED_SUBST_BACKQUOTE,
	SH_E_UNTERMINATED_SUBST_BACKQUOTE_REV,
	SH_E_UNTERMINATED_BRACKET,
	SH_E_UNTERMINATED_IF,
	SH_E_UNTERMINATED_THEN,
	SH_E_UNTERMINATED_ELIF,
	SH_E_UNTERMINATED_ELSE,
	SH_E_UNTERMINATED_WHILE,
	SH_E_UNTERMINATED_FOR,
	SH_E_UNTERMINATED_IN,
	SH_E_UNTERMINATED_DO,
	SH_E_UNTERMINATED_LINE,
	SH_E_UNTERMINATED_STRING,
	SH_E_UNTERMINATED_STRING_SINGLE,
	SH_E_UNTERMINATED_STRING_ANSI,
};

struct			s_sh_parse_err
{
	t_sh_parse_err_t	type;
	union {
		t_sub						token;
		t_sh_parse_err_unterminated	unterminated;
	};
};

/*
** Parse a full line + heredoc
** -
** On success return NULL and fill 'dst' with the parsed command
** On error return an error object
** -
** The returned error object can be freed using free
*/
t_sh_parse_err	*sh_parse(t_in *in, t_sh_compound *dst);

#endif
