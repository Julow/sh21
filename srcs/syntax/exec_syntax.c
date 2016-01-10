/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2016/01/10 00:03:11 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_dstr.h"

#include "syntax.h"

struct s_exec_syntax // TODO: move
{
	t_sub			line;
	t_sub			token;
	t_dstr			scope;
	void			(*callback)(t_sub, t_sub, void*);
	void			*env;
};

static void		exec(struct s_exec_syntax *s, t_syntax const *syntax);

static bool		exec_token(struct s_exec_syntax *s, t_syntax_scope const *scope)
{
	uint32_t		top;

	top = s->scope.length;
	if (scope->syntax != NULL)
	{
		ft_dstradd(&s->scope, scope->syntax->scope);
		ft_dstradd(&s->scope, SUBC("."));
	}
	ft_dstradd(&s->scope, scope->name);
	if (s->token.length > 0)
		s->callback(s->token, SUB(s->scope.str, s->scope.length), s->env);
	s->scope.length = top;
	if (scope->syntax != NULL)
		exec(s, scope->syntax);
	s->scope.length = top;
	return (scope->end);
}

static bool		exec_match(struct s_exec_syntax *s, t_syntax const *syntax)
{
	t_sub			match;
	t_syntax_match	*m;
	uint32_t		i;

	i = 0;
	while (i < syntax->match.length)
	{
		m = VECTOR_GET(syntax->match, i);
		match = SUB(s->token.str, 0);
		if (ft_rsearch(s->token, &match, &m->regex, NULL))
		{
			if (match.str > s->token.str)
				s->callback(SUB(s->token.str, match.str - s->token.str),
					SUB(s->scope.str, s->scope.length), s->env);
			s->token = match;
			return (exec_token(s, m->scope));
		}
		i++;
	}
	if (s->token.length > 0)
		s->callback(s->token, SUB(s->scope.str, s->scope.length), s->env);
	return (false);
}

static void		exec(struct s_exec_syntax *s, t_syntax const *syntax)
{
	t_syntax_scope	*scope;

	ft_dstradd(&s->scope, syntax->scope);
	ft_dstradd(&s->scope, SUBC("."));
	while (ft_tokenize(s->line, &s->token, (void**)&scope, &syntax->token_map))
		if ((scope == NULL) ? exec_match(s, syntax) : exec_token(s, scope))
			break ;
}

void			exec_syntax(t_sub line, void (*callback)(),
					t_syntax const *syntax, void *env)
{
	struct s_exec_syntax	s;

	s = (struct s_exec_syntax){
		line,
		SUB(line.str, 0),
		DSTR0(),
		callback,
		env
	};
	exec(&s, syntax);
	ft_dstrclear(&s.scope);
}
