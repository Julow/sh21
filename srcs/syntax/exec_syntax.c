/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2016/01/19 15:54:43 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_dstr.h"

#include "syntax.h"

struct			s_exec_syntax // TODO: move
{
	t_sub			line;
	t_sub			token;
	void			(*callback)(t_sub, t_syntax_data*, void*);
	void			*env;
};

static void		exec(struct s_exec_syntax *s, t_syntax const *syntax,
					t_syntax_data *data);

static bool		exec_token(struct s_exec_syntax *s, t_syntax_token const *token,
					t_syntax_data *prev_data)
{
	t_syntax_data	data;
	t_syntax_data	syntax_data;

	if (token->syntax != NULL)
	{
		syntax_data = (t_syntax_data){token->syntax->data, prev_data};
		if (s->token.length > 0)
		{
			data = (t_syntax_data){token->data, &syntax_data};
			s->callback(s->token, &data, s->env);
		}
		exec(s, token->syntax, &syntax_data);
	}
	else if (s->token.length > 0)
	{
		data = (t_syntax_data){token->data, prev_data};
		s->callback(s->token, &data, s->env);
	}
	return (token->end);
}

static bool		exec_match(struct s_exec_syntax *s, t_syntax const *syntax,
					t_syntax_data *prev_data)
{
	t_sub			match;
	t_syntax_data	nomatch_data;
	t_syntax_match	*m;
	uint32_t		i;

	nomatch_data = (t_syntax_data){NULL, prev_data};
	i = 0;
	while (i < syntax->match.length)
	{
		m = VECTOR_GET(syntax->match, i);
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

static void		exec(struct s_exec_syntax *s, t_syntax const *syntax,
					t_syntax_data *data)
{
	t_syntax_token	*token;

	while (ft_tokenize(s->line, &s->token, (void**)&token, &syntax->token_map))
		if ((token == NULL)
				? exec_match(s, syntax, data)
				: exec_token(s, token, data))
			break ;
}

void			exec_syntax(t_sub line, void (*callback)(),
					t_syntax const *syntax, void *env)
{
	struct s_exec_syntax	s;
	t_syntax_data			data;

	s = (struct s_exec_syntax){
		line,
		SUB(line.str, 0),
		callback,
		env
	};
	data = (t_syntax_data){syntax->data, NULL};
	exec(&s, syntax, &data);
}
