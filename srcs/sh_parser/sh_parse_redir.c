/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/15 17:58:07 by juloo             #+#    #+#             */
/*   Updated: 2016/08/17 20:38:05 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static bool		parse_redir(t_sh_parser *p, t_sh_redir_lst *dst,
					int32_t left_fd)
{
	t_sh_redir			*redir;
	uint32_t const		text_offset = dst->text.tokens.length;

	ASSERT(SH_T(p)->type == SH(REDIR));
	redir = ft_vpush(&dst->redirs, NULL, 1);
	*redir = SH_REDIR(SH_T(p)->redir, left_fd);
	sh_ignore_spaces(p);
	while (ft_lexer_next(&p->l) && SH_T(p)->type != SH(SPACE)
			&& g_sh_parse_text[SH_T(p)->type] != NULL)
	{
		if (!g_sh_parse_text[SH_T(p)->type](p, &dst->text))
			return (false);
	}
	redir->right_len = dst->text.tokens.length - text_offset;
	return (redir->right_len > 0
		|| sh_parse_error(p, p->l.eof ? SH_E_EOF : SH_E_UNEXPECTED));
}

bool			sh_parse_redir(t_sh_parser *p, t_sh_redir_lst *lst)
{
	ASSERT(SH_T(p)->type == SH(REDIR));
	return (parse_redir(p, lst, -1));
}

bool			sh_parse_redir_left(t_sh_parser *p,
					t_sh_redir_lst *lst, bool *r)
{
	int32_t					left_fd;
	t_sh_parse_token const	*t;

	ASSERT(SH_T(p)->type == SH(TEXT));
	if (ft_subto_int(p->l.t.token, &left_fd) != p->l.t.token.length || left_fd < 0
		|| !ft_lexer_ahead(&p->l, NULL, V(&t)) || t->type != SH(REDIR))
		return (false);
	ft_lexer_next(&p->l);
	*r = parse_redir(p, lst, left_fd);
	return (true);
}


#define _BIT_FIELD(T, ...)	(1 << SH(T))
#define BIT_FIELD(...)		(FOR_EACH(_BIT_FIELD, |, , ##__VA_ARGS__))

static uint32_t const	g_parse_trailing_stop = BIT_FIELD(
	COMMENT,
	COMPOUND_END,
	LIST_END,
	PIPELINE_END
);

bool			sh_parse_trailing_redirs(t_sh_parser *p, t_sh_redir_lst *lst)
{
	bool			r;

	if (!ft_lexer_next(&p->l))
		return (true);
	r = true;
	while (r && !p->l.eof)
		if (SH_T(p)->type == SH(REDIR))
			r = sh_parse_redir(p, lst);
		else if (SH_T(p)->type == SH(HEREDOC))
			r = ASSERT(!"TODO: heredoc");
		else if (SH_T(p)->type == SH(TEXT))
			sh_parse_redir_left(p, lst, &r)
				|| (r = sh_parse_error(p, SH_E_UNEXPECTED));
		else if (g_parse_trailing_stop & (1 << SH_T(p)->type))
			break ;
		else if (SH_T(p)->type != SH(SPACE))
			r = sh_parse_error(p, SH_E_UNEXPECTED);
		else
			ft_lexer_next(&p->l);
	return (r);
}
