/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2016/02/02 17:47:51 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/parser.h"

struct			s_exec_parser // TODO: move
{
	t_tokenizer		tokenizer;
	t_callback		on_parser_start;
	t_callback		on_parser_end;
	t_callback		on_token;
	uint32_t		data_size;
};

static void		exec(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *data);

static bool		exec_token(struct s_exec_parser *s, t_parser_token const *token,
					t_parser_data *parent_data)
{
	t_parser_data		parser_data;
	uint8_t				data[s->data_size];

	if (token->parser != NULL)
	{
		parser_data = (t_parser_data){data, parent_data, NULL};
		parent_data->next = &parser_data;
		CALL(void, s->on_parser_start, &parser_data, token->parser->data);
		CALL(void, s->on_token, &parser_data, s->tokenizer.token, token->data);
		exec(s, token->parser, &parser_data);
		CALL(void, s->on_parser_end, &parser_data, token->parser->data);
		parent_data->next = NULL;
	}
	else
		CALL(void, s->on_token, parent_data, s->tokenizer.token, token->data);
	return (token->end);
}

static bool		exec_match(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *parent_data)
{
	t_sub			match;
	t_parser_match	*m;
	uint32_t		i;

	i = 0;
	while (i < parser->match.length)
	{
		m = VECTOR_GET(parser->match, i);
		match = SUB(s->tokenizer.token.str, 0);
		if (ft_rsearch(s->tokenizer.token, &match, &m->regex, NULL))
		{
			if (SUB_OFF(s->tokenizer.token, match) > 0)
				CALL(void, s->on_token, parent_data,
					SUB_BEF(s->tokenizer.token, match), NULL);
			s->tokenizer.end -= s->tokenizer.token.length
				- (SUB_OFF(s->tokenizer.token, match) + match.length);
			s->tokenizer.token = match;
			return (exec_token(s, &m->token, parent_data));
		}
		i++;
	}
	if (s->tokenizer.token.length > 0)
		CALL(void, s->on_token, parent_data, s->tokenizer.token, NULL);
	return (false);
}

static void		exec(struct s_exec_parser *s, t_parser const *parser,
					t_parser_data *data)
{
	t_token_map const	*old_map = s->tokenizer.token_map;

	s->tokenizer.token_map = &parser->token_map;
	while (ft_tokenize(&s->tokenizer))
		if ((s->tokenizer.token_data == NULL)
				? exec_match(s, parser, data)
				: exec_token(s, s->tokenizer.token_data, data))
			break ;
	s->tokenizer.token_map = old_map;
}

void			exec_parser(t_in *in, t_parser const *parser,
					t_callback callbacks[3], uint32_t data_size)
{
	struct s_exec_parser	s;
	t_parser_data			parser_data;
	uint8_t					data[data_size];

	s = (struct s_exec_parser){
		TOKENIZER(in, NULL),
		callbacks[0],
		callbacks[1],
		callbacks[2],
		data_size
	};
	parser_data = (t_parser_data){data, NULL, NULL};
	CALL(void, s.on_parser_start, &parser_data, parser->data);
	exec(&s, parser, &parser_data);
	CALL(void, s.on_parser_end, &parser_data, parser->data);
	D_TOKENIZER(s.tokenizer);
}
