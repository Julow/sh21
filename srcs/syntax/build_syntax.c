/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 20:06:51 by juloo             #+#    #+#             */
/*   Updated: 2016/01/07 23:08:14 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"
#include "syntax_def.h"

static t_syntax	*get_syntax(t_sub name, t_hmap *map, t_vector const *syntaxes);

static void		add_token(t_syntax *syntax, t_sub token, t_sub scope_def,
					t_syntax *sub_syntax, bool end_token)
{
	t_token_def		token_def;
	t_syntax_scope	*scope;

	scope = ft_emalloc(sizeof(t_syntax_scope) + scope_def.length);
	ft_memcpy(ENDOF(scope), scope_def.str, scope_def.length);
	scope->name = SUB(ENDOF(scope), scope_def.length);
	scope->syntax = sub_syntax;
	token_def.sub = token;
	token_def.data = scope;
	ft_token_map(&syntax->token_map, &token_def);
	if (end_token)
		syntax->end_token = scope;
}

static void		add_match(t_syntax *syntax, t_sub pattern, t_sub scope_def,
					t_syntax *sub_syntax, bool end_token)
{
	t_syntax_match	match;
	t_regex_err		err;

	if (!ft_rcompile(&match.regex, pattern, &err))
	{
		ft_printf("%ts: '%ts'\n%.*c^%n",
			err.str, pattern, err.str.length + 3, ' ');
		return ;
	}
	match.scope = ft_emalloc(sizeof(t_syntax_scope) + scope_def.length);
	ft_memcpy(ENDOF(match.scope), scope_def.str, scope_def.length);
	match.scope->name = SUB(ENDOF(match.scope), scope_def.length);
	match.scope->syntax = sub_syntax;
	ft_vpush_back(&syntax->match, &match, 1);
	if (end_token)
		syntax->end_token = match.scope;
}

static inline bool	ft_subequ(t_sub a, t_sub b)
{
	return (BOOL_OF(a.length == b.length
		&& ft_memcmp(a.str, b.str, b.length) == 0));
}

static bool		dupplicate_token(t_token_def const *token, t_sub const *name,
					void *env)
{
	if (ft_subequ(token->sub, *name))
		return (false);
	return (true);
	(void)env;
}

static bool		syntax_inherit(t_token_def const *token, t_syntax *syntax)
{
	t_syntax_scope *const	p_scope = token->data;

	if (ft_bst_getall(&syntax->token_map.tokens, &token->sub, &dupplicate_token,
			NULL))
		add_token(syntax, token->sub, p_scope->name, p_scope->syntax, false);
	return (true);
}

static t_syntax	*build(t_hmap *map, t_vector const *syntaxes,
					t_syntax_def const *def)
{
	t_syntax				*syntax;
	t_syntax				*parent;
	t_syntax				*sub_syntax;
	t_syntax_def_t const	*token;
	int						i;

	ft_hmapputp(map, def->name, NULL);
	parent = NULL;
	if (def->inherit.length > 0
		&& (parent = get_syntax(def->inherit, map, syntaxes)) == NULL)
		return (NULL);
	syntax = ft_hmapput(map, def->name, NULL,
		sizeof(t_syntax) + def->scope.length).value;
	ft_memcpy(ENDOF(syntax), def->scope.str, def->scope.length);
	syntax->scope = SUB(ENDOF(syntax), def->scope.length);
	syntax->end_token = NULL;
	syntax->token_map = TOKEN_MAP();
	syntax->match = VECTOR(t_syntax_match);
	i = 0;
	while (i < def->tokens.length)
	{
		token = VECTOR_GET(def->tokens, i);
		sub_syntax = NULL;
		if (token->syntax.length > 0
			&& (sub_syntax = get_syntax(token->syntax, map, syntaxes)) == NULL)
			return (NULL);
		add_token(syntax, token->token, token->scope, sub_syntax,
			BOOL_OF(def->end.length > 0 && ft_subequ(def->end, token->scope)));
		i++;
	}
	i = 0;
	while (i < def->match.length)
	{
		token = VECTOR_GET(def->match, i);
		sub_syntax = NULL;
		if (token->syntax.length > 0
			&& (sub_syntax = get_syntax(token->syntax, map, syntaxes)) == NULL)
			return (NULL);
		add_match(syntax, token->token, token->scope, sub_syntax,
			BOOL_OF(def->end.length > 0 && ft_subequ(def->end, token->scope)));
		i++;
	}
	if (parent != NULL)
		ft_bst_iter(&parent->token_map.tokens, &syntax_inherit, syntax);
	if (def->end.length > 0 && syntax->end_token == NULL)
	{
		ft_dprintf(2, "Missing end token: '%ts' (%ts)%n", def->end, def->name);
		return (NULL);
	}
	return (syntax);
}

static t_syntax	*get_syntax(t_sub name, t_hmap *map, t_vector const *syntaxes)
{
	t_hpair				tmp;
	t_syntax_def const	*def;
	int					i;

	if ((tmp = ft_hmapget(map, name)).key != NULL)
	{
		if (tmp.value == NULL)
			ft_dprintf(2, "Inherit loop: %ts%n", name);
		return (tmp.value);
	}
	i = 0;
	while (i < syntaxes->length)
	{
		def = VECTOR_GET(*syntaxes, i);
		if (def->name.length == name.length
			&& ft_memcmp(def->name.str, name.str, name.length) == 0)
			return (build(map, syntaxes, def));
		i++;
	}
	ft_dprintf(2, "Unknown syntax: %ts%n", name);
	return (NULL);
}

bool			build_syntax(t_hmap *dst, t_vector const *syntaxes)
{
	int				i;

	i = 0;
	while (i < syntaxes->length)
		if (build(dst, syntaxes, VECTOR_GET(*syntaxes, i++)) == NULL)
			return (false);
	return (true);
}
