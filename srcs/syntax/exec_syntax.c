/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/19 20:18:26 by juloo             #+#    #+#             */
/*   Updated: 2015/12/20 00:12:29 by juloo            ###   ########.fr       */
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

static void		exec(struct s_exec_syntax *s, t_syntax const *syntax)
{
	uint32_t		top;
	t_syntax_scope	*scope;

	ft_dstradd(&s->scope, syntax->scope);
	ft_dstradd(&s->scope, SUBC("."));
	top = s->scope.length;
	while (ft_tokenize(s->line, &s->token, (void**)&scope, &syntax->token_map))
	{
		if (scope != NULL)
		{
			if (scope->syntax != NULL)
			{
				ft_dstradd(&s->scope, scope->syntax->scope);
				ft_dstradd(&s->scope, SUBC("."));
			}
			ft_dstradd(&s->scope, scope->name);
		}
		s->callback(s->token, SUB(s->scope.str, s->scope.length), s->env);
		s->scope.length = top;
		if (scope != NULL)
		{
			if (scope->syntax != NULL)
				exec(s, scope->syntax);
			s->scope.length = top;
			if (scope == syntax->end_token)
				break ;
		}
	}
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
