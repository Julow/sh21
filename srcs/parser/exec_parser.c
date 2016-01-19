/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2016/01/19 16:56:56 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/parser.h"

struct			s_exec_parser // TODO: move
{
	t_sub			line;
	t_sub			token;
	void			(*callback)(t_sub, t_parser_data*, void*);
	void			*env;
};

static void		exec(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *data);

static bool		exec_token(struct s_exec_parser *s, t_parser_token const *token,
					t_parser_data *prev_data)
{
	t_parser_data	data;
	t_parser_data	parser_data;

	if (token->parser != NULL)
	{
		parser_data = (t_parser_data){token->parser->data, prev_data};
		if (s->token.length > 0)
		{
			data = (t_parser_data){token->data, &parser_data};
			s->callback(s->token, &data, s->env);
		}
		exec(s, token->parser, &parser_data);
	}
	else if (s->token.length > 0)
	{
		data = (t_parser_data){token->data, prev_data};
		s->callback(s->token, &data, s->env);
	}
	return (token->end);
}

static bool		exec_match(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *prev_data)
{
	t_sub			match;
	t_parser_data	nomatch_data;
	t_parser_match	*m;
	uint32_t		i;

	nomatch_data = (t_parser_data){NULL, prev_data};
	i = 0;
	while (i < parser->match.length)
	{
		m = VECTOR_GET(parser->match, i);
		match = SUB(s->token.str, 0);
		if (ft_rsearch(s->token, &match, &m->regex, NULL))
		{
			if (SUB_OFF(s->token, match) > 0)
				s->callback(SUB_BEF(s->token, match), &nomatch_data, s->env);
			s->token = match;
			return (exec_token(s, &m->token, prev_data));
		}
		i++;
	}
	if (s->token.length > 0)
		s->callback(s->token, &nomatch_data, s->env);
	return (false);
}

static void		exec(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *data)
{
	t_parser_token	*token;

	while (ft_tokenize(s->line, &s->token, (void**)&token, &parser->token_map))
		if ((token == NULL)
				? exec_match(s, parser, data)
				: exec_token(s, token, data))
			break ;
}

void			exec_parser(t_sub line, void (*callback)(),
					t_parser const *parser, void *env)
{
	struct s_exec_parser	s;
	t_parser_data			data;

	s = (struct s_exec_parser){
		line,
		SUB(line.str, 0),
		callback,
		env
	};
	data = (t_parser_data){parser->data, NULL};
	exec(&s, parser, &data);
}
