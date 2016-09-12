/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_compound.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 11:23:04 by juloo             #+#    #+#             */
/*   Updated: 2016/09/12 16:54:04 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static bool		sh_parse_pipeline(t_sh_parser *p, t_sh_pipeline *dst)
{
	if (!sh_parse_cmd(p, &dst->cmd))
		return (false);
	dst->next = NULL;
	if (p->t.eof || SH_T(p)->type != SH(PIPELINE_END))
		return (true);
	if (sh_ignore_newlines(p))
	{
		dst->next = NEW(t_sh_pipeline);
		if (sh_parse_pipeline(p, dst->next))
			return (true);
		free(dst->next);
	}
	else
		sh_parse_error_unterminated(p, SH_E_UNTERMINATED_PIPE);
	sh_destroy_cmd(&dst->cmd);
	return (false);
}

static bool		sh_parse_list(t_sh_parser *p, t_sh_list *dst)
{
	t_sh_list_next_t	next_t;

	if (!sh_parse_pipeline(p, &dst->pipeline))
		return (false);
	dst->next = NULL;
	if (p->t.eof || SH_T(p)->type != SH(LIST_END))
		return (true);
	next_t = SH_T(p)->list_end;
	if (sh_ignore_newlines(p))
	{
		dst->next = NEW(t_sh_list_next);
		dst->next->type = next_t;
		if (sh_parse_list(p, &dst->next->next))
			return (true);
		free(dst->next);
	}
	else
		sh_parse_error_unterminated(p, (next_t == SH_LIST_AND) ?
				SH_E_UNTERMINATED_AND : SH_E_UNTERMINATED_OR);
	sh_destroy_pipeline(&dst->pipeline);
	return (false);
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

bool			sh_parse_compound(t_sh_parser *p, t_sh_compound *dst,
					bool allow_newline)
{
	dst->flags = 0;
	dst->next = NULL;
	if (!sh_parse_list(p, &dst->list))
		return (false);
	if (p->t.eof)
		return (true);
	if (SH_T(p)->type == SH(COMPOUND_END))
	{
		if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_AMPERSAND)
			dst->flags |= SH_COMPOUND_ASYNC;
		else if (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_SUBSHELL
			|| (SH_T(p)->compound_end == SH_PARSE_T_COMPOUND_NEWLINE
				&& !allow_newline))
			return (true);
		if (sh_parse_compound_end(p) || p->t.eof)
			return (true);
		dst->next = NEW(t_sh_compound);
		if (sh_parse_compound(p, dst->next, allow_newline))
			return (true);
		free(dst->next);
	}
	else
		sh_parse_error(p, SH_E_UNEXPECTED);
	sh_destroy_list(&dst->list);
	return (false);
}
