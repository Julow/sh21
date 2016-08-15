/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:21:17 by juloo             #+#    #+#             */
/*   Updated: 2016/08/14 16:47:55 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

bool			ft_subis(t_sub sub, t_is is)
{
	char const *const	end = sub.str + sub.length;
	char const			*str;

	str = sub.str;
	while (str < end)
	{
		if (!IS(*str, is))
			return (false);
		str++;
	}
	return (true);
}

bool			sh_except_token(t_sh_parser *p, t_sh_parse_token t)
{
	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	if (SH_T(p) == NULL || SH_T(p)->type != t.type || SH_T(p)->_val != t._val)
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	return (true);
}

bool			sh_ignore_spaces(t_sh_parser *p)
{
	t_sh_parse_token const	*t;

	while (ft_lexer_ahead(&p->l, NULL, V(&t)))
	{
		if (t == NULL || t->type != SH_PARSE_T_SPACE)
			return (true);
		if (!ft_lexer_next(&p->l)
			|| (t = SH_T(p)) == NULL || t->type != SH_PARSE_T_SPACE)
			ASSERT(!"Lexer ahead/next mismatch"); // TODO: remove
	}
	return (false);
}

bool			sh_ignore_newlines(t_sh_parser *p)
{
	t_sh_parse_token const	*t;

	while (ft_lexer_ahead(&p->l, NULL, V(&t)))
	{
		if (t == NULL || (t->type != SH_PARSE_T_SPACE
				&& !(t->type == SH_PARSE_T_COMPOUND_END
					&& t->compound_end == SH_PARSE_T_COMPOUND_NEWLINE)))
			return (true);
		ft_lexer_next(&p->l);
	}
	ft_lexer_next(&p->l);
	return (false);
}

bool			sh_parse_error(t_sh_parser *p, t_sh_parse_err_t t)
{
	if (ASSERT(!p->error_set, "Double error") && p->err != NULL)
		p->err->err = t;
	p->error_set = true;
	return (false);
}
