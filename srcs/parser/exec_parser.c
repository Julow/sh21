/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2016/02/09 14:15:58 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/parser.h"

static bool		exec_frame(t_parse_data *p, t_parser const *parser)
{
	t_parse_frame			frame;
	bool					ret;

	frame = (t_parse_frame){parser, NULL, p->frame};
	p->frame = &frame;
	p->t.token_map = &parser->token_map;
	ret = parser->f(p);
	p->t.token_map = (frame.prev == NULL ) ? NULL
		: &frame.prev->parser->token_map;
	p->frame = frame.prev;
	return (ret);
}

static bool		exec_token(t_parse_data *p,
					t_sub *token_str, void const **token_data)
{
	t_parser_token const *const	t = p->t.token_data;

	if (t->parser != NULL)
	{
		if (token_str != NULL)
			*token_str = SUB0();
		if (token_data != NULL)
			*token_data = NULL;
		if (!exec_frame(p, t->parser))
			return (false);
		return (t->end ? false : parse_token(p, token_str, token_data));
	}
	if (token_str != NULL)
		*token_str = p->t.token;
	if (token_data != NULL)
		*token_data = p->t.token_data;
	return (!t->end);
}

static bool		exec_match(t_parse_data *p,
					t_sub *token_str, void const **token_data)
{
	t_sub			match;
	t_parser_match	*m;
	uint32_t		i;

	i = 0;
	while (i < p->frame->parser->match.length)
	{
		m = VECTOR_GET(p->frame->parser->match, i);
		match = SUB(p->t.token.str, 0);
		if (ft_rsearch(p->t.token, &match, &m->regex, NULL))
		{
			if (SUB_OFF(p->t.token, match) > 0)
			{
				i = SUB_OFF(p->t.token, match);
				p->t.end -= p->t.token.length - i;
				p->t.token.length = i;
				return (exec_match(p, token_str, token_data));
			}
			p->t.end -= p->t.token.length - match.length;
			p->t.token = match;
			return (exec_token(p, token_str, token_data));
		}
		i++;
	}
	if (token_str != NULL)
		*token_str = p->t.token;
	if (token_data != NULL)
		*token_data = p->t.token_data;
	return (true);
}

bool			parse_token(t_parse_data *p,
					t_sub *token_str, void const **token_data)
{
	if (!ft_tokenize(&p->t))
		return (false);
	if (p->t.token_data == NULL)
		return (exec_match(p, token_str, token_data));
	return (exec_token(p, token_str, token_data));
}

bool			parse(t_in *in, t_parser const *parser, void *env)
{
	t_parse_data	p;
	bool			ret;

	p = (t_parse_data){
		env,
		TOKENIZER(in, NULL),
		NULL,
		false
	};
	ret = exec_frame(&p, parser);
	D_TOKENIZER(p.t);
	return (ret);
}
