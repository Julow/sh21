/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 20:06:51 by juloo             #+#    #+#             */
/*   Updated: 2015/12/19 15:27:25 by juloo            ###   ########.fr       */
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
	scope->scope = SUB(ENDOF(scope), scope_def.length);
	scope->syntax = sub_syntax;
	token_def.sub = token;
	token_def.data = scope;
	ft_token_map(&syntax->token_map, &token_def);
	if (end_token)
		syntax->end_token = scope;
}

static bool		syntax_inherit(t_token_def const *token, t_syntax *syntax)
{
	t_syntax_scope *const	p_scope = token->data;

	add_token(syntax, token->sub, p_scope->scope, p_scope->syntax, false);
	return (true);
}

static inline bool	ft_subequ(t_sub a, t_sub b)
{
	return (BOOL_OF(a.length == b.length
		&& ft_memcmp(a.str, b.str, b.length) == 0));
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
	syntax = ft_hmapput(map, def->name, NULL, sizeof(t_syntax)).value;
	*syntax = (t_syntax){NULL, TOKEN_MAP()};
	if (parent != NULL)
		ft_bst_iter(&parent->token_map.tokens, &syntax_inherit, syntax);
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
