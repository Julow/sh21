/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_compound.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:04 by juloo             #+#    #+#             */
/*   Updated: 2016/09/09 13:04:03 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static bool		sh_parse_pipeline(t_sh_parser *p, t_sh_pipeline *dst)
{
	while (true)
	{
		dst->next = NULL;
		if (!sh_parse_cmd(p, &dst->cmd))
			return (false);
		if (p->t.eof || SH_T(p)->type != SH(PIPELINE_END))
			break ;
		if (!sh_ignore_newlines(p))
			return (sh_parse_error(p, SH_E_EOF));
		dst->next = NEW(t_sh_pipeline);
		dst = dst->next;
	}
	return (true);
}

static bool		sh_parse_list(t_sh_parser *p, t_sh_list *dst)
{
	t_sh_list_next_t	next_t;

	while (true)
	{
		dst->next = NULL;
		if (!sh_parse_pipeline(p, &dst->pipeline))
			return (false);
		if (p->t.eof || SH_T(p)->type != SH(LIST_END))
			break ;
		next_t = SH_T(p)->list_end;
		if (!sh_ignore_newlines(p))
			return (sh_parse_error(p, SH_E_EOF));
		dst->next = NEW(t_sh_list_next);
		dst->next->type = next_t;
		dst = &dst->next->next;
	}
	return (true);
}

static struct {
	t_sub				name;
	t_sh_parse_token	t;
} const			g_end_keywords[] = {
	{SUBC("do"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_DO)}}},
	{SUBC("done"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_DONE)}}},
	{SUBC("then"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_THEN)}}},
	{SUBC("else"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_ELSE)}}},
	{SUBC("elif"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_ELIF)}}},
	{SUBC("fi"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_FI)}}},
	{SUBC("}"), {SH(COMPOUND_END), {.compound_end=SH(COMPOUND_BRACKET)}}},
};

bool			sh_parse_compound_end(t_sh_parser *p)
{
	uint32_t				i;
	t_sub					word;
	t_sh_parse_token const	*t;

	i = 0;
	if ((SH_T(p)->compound_end == SH(COMPOUND_SEMICOLON)
			|| SH_T(p)->compound_end == SH(COMPOUND_NEWLINE))
		&& sh_ignore_newlines(p)
		&& ft_tokenize_ahead(&p->t, &word, V(&t)) && t->type == SH(TEXT))
		while (i < ARRAY_LEN(g_end_keywords))
		{
			if (SUB_EQU(g_end_keywords[i].name, word))
			{
				ft_tokenize(&p->t);
				p->t.token = &g_end_keywords[i].t;
				return (true);
			}
			i++;
		}
	sh_ignore_newlines(p);
	return (false);
}

// TODO: destroy on error
bool			sh_parse_compound(t_sh_parser *p, t_sh_compound *dst,
					bool allow_newline)
{
	while (true)
	{
		dst->flags = 0;
		dst->next = NULL;
		if (!sh_parse_list(p, &dst->list))
			return (false);
		if (p->t.eof || !ASSERT(SH_T(p)->type == SH(COMPOUND_END)))
			break ;
		if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_AMPERSAND)
			dst->flags |= SH_COMPOUND_ASYNC;
		else if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_SUBSHELL
			|| (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_NEWLINE
				&& !allow_newline))
			break ;
		if (sh_parse_compound_end(p) || p->t.eof)
			break ;
		dst->next = NEW(t_sh_compound);
		dst = dst->next;
	}
	return (true);
}
