/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/02/01 19:33:37 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_hmap.h"
#include "ft/ft_printf.h"
#include "ft/get_next_line.h"
#include "ft/getkey.h"
#include "ft/parser.h"
#include "ft/parser_def.h"
#include "ft/spanlist.h"
#include "ft/term.h"
#include "ft/tokenizer.h"

#include "editor.h"

#include <termios.h>
#include <unistd.h>

#include <sys/select.h>

/*
** TODO: multi line (ctrl+n)
** TODO: multi cursor
** TODO: sequenced binding (like ctrl+K,ctrl+C)
** TODO: next matching binding on binding returning false
** TODO: undo/redo history
** TODO: signals
*/

#define WARNING_MSG(MSG)	(C_YELLOW "Warning" C_RESET ": " MSG "%n")

/*
** ========================================================================== **
** Main
*/

typedef struct s_main		t_main;

#define FLAG_INTERACTIVE	(1 << 0)
#define FLAG_EXIT			(1 << 1)

struct			s_main
{
	t_term				*term;
	t_editor			*editor;
	uint32_t			flags;
	t_hmap				*parsers;
	t_parser			*curr_parser;
};

/*
** ========================================================================== **
** Debug
*/

struct			s_keyname
{
	int				key;
	char const		*name;
};

static char const	*get_key_name(t_key key)
{
	static struct s_keyname		key_names[] = {
		{KEY_BACKSPACE, "backspace"},
		{KEY_ESC, "esc"},
		{KEY_UP, "up"},
		{KEY_RIGHT, "right"},
		{KEY_DOWN, "down"},
		{KEY_LEFT, "left"},
		{KEY_DELETE, "delete"},
		{KEY_HOME, "home"},
		{KEY_END, "end"},
		{KEY_PAGEUP, "pageup"},
		{KEY_PAGEDOWN, "pagedown"},
		{KEY_F1, "f1"},
		{KEY_F2, "f2"},
		{KEY_F3, "f3"},
		{KEY_F4, "f4"},
		{'\t', "tab"},
	};
	uint32_t		i;

	i = 0;
	while (i < ARRAY_LEN(key_names))
	{
		if (key_names[i].key == key.c)
			return (key_names[i].name);
		i++;
	}
	return (NULL);
}

static void		put_key(t_out *out, t_key key)
{
	char const		*key_name;

	ft_putsub(out, SUBC("Key: "));
	if (key.mods & KEY_MOD_CTRL)
		ft_putsub(out, SUBC("ctrl+"));
	if (key.mods & KEY_MOD_ALT)
		ft_putsub(out, SUBC("alt+"));
	if (key.mods & KEY_MOD_SHIFT)
		ft_putsub(out, SUBC("shift+"));
	if (IS(key.c, IS_PRINT))
		ft_fprintf(out, "%c", key.c);
	else if ((key_name = get_key_name(key)) != NULL)
		ft_fprintf(out, "%s", key_name);
	else
		ft_fprintf(out, "0x%0.2x", (uint32_t)key.c);
	ft_putendl(out);
}

/*
** ========================================================================== **
** Debug parser
*/
// mdr (truc "loool''" 'xd" $LOL $(mdr)' $(( 5 + 4/(8.0-3.f))) "$(xd "mdr$A" '$lol')$LOL") lol

/*
** Check if 'str' match 'pattern'
** Pattern parser:
**   '*'	match 0 or more of any char
**   '?'	match any char
** If 'match' is NULL, check the whole string
** If 'match' is not NULL, try to match a sub string of 'str'
** 'match' should be initialized to SUB(str.str, 0)
*/
// static uint32_t	match_str(t_sub str, t_sub pattern);

// static uint32_t	match_wildcard(t_sub str, t_sub pattern, uint32_t i)
// {
// 	uint32_t		tmp;
// 	uint32_t		max;

// 	tmp = i;
// 	while (++tmp < pattern.length)
// 		if (pattern.str[tmp] == '?')
// 			i++;
// 		else if (!(pattern.str[tmp] == '*'))
// 			break ;
// 	if (tmp >= pattern.length)
// 		return (str.length);
// 	pattern = SUB(pattern.str + tmp, pattern.length - tmp);
// 	max = 0;
// 	while (i < str.length)
// 	{
// 		if (str.str[i] == pattern.str[0])
// 		{
// 			tmp = match_str(SUB(str.str + i, str.length - i), pattern) + i;
// 			if (tmp > i && tmp > max)
// 				max = tmp;
// 		}
// 		i++;
// 	}
// 	return (max);
// }

// static uint32_t	match_str(t_sub str, t_sub pattern)
// {
// 	uint32_t		len;
// 	uint32_t		i;

// 	i = 0;
// 	len = MIN(str.length, pattern.length);
// 	while (i < len)
// 	{
// 		if (pattern.str[i] == '*')
// 			return (match_wildcard(str, pattern, i));
// 		if (str.str[i] != pattern.str[i] && pattern.str[i] != '?')
// 			return (0);
// 		i++;
// 	}
// 	while (i < pattern.length && pattern.str[i] == '*')
// 		pattern = SUB(pattern.str + 1, pattern.length - 1);
// 	return ((i == pattern.length) ? i : 0);
// }

// bool			ft_match(t_sub str, t_sub *match, t_sub pattern)
// {
// 	uint32_t		i;
// 	uint32_t		tmp;

// 	if (match == NULL)
// 		return (BOOL_OF(match_str(str, pattern) == str.length));
// 	i = match->str + match->length - str.str;
// 	tmp = 0;
// 	while (i < str.length)
// 	{
// 		tmp = match_str(SUB(str.str + i, str.length - i), pattern);
// 		if (tmp > 0)
// 			break ;
// 		i++;
// 	}
// 	match->str = str.str + i;
// 	match->length = tmp;
// 	return (BOOL_OF(tmp > 0));
// }

/*
** ========================================================================== **
*/

#define SCOPE(S,C)	{SUBC(S), STYLE C}

static struct {
	t_sub			scope;
	t_style			style;
} const			g_colors[] = {
	SCOPE("string.simple", (S_YELLOW, 0, 0)),
	SCOPE("string", (S_LIGHT(S_YELLOW), 0, 0)),
	SCOPE("escaped", (S_RED, 0, 0)),
	SCOPE("number", (S_YELLOW, 0, 0)),
	SCOPE("var", (S_RED, 0, 0)),
	SCOPE("comment", (0, S_BLUE, 0)),
	SCOPE("op", (S_LIGHT(S_WHITE), 0, 0)),
};

static void		on_parser_start(void *env, t_parser_data *data, void const *p)
{
	data->data = p;
	(void)env;
}

static void		on_parser_end(void *env, t_parser_data *data, void const *p)
{
	(void)env;
	(void)data;
	(void)p;
}

static t_style	get_color(t_parser_data *data)
{
	uint32_t		i;
	t_sub			sub;

	while (data != NULL)
	{
		sub = ft_sub(data->data, 0, -1);
		i = 0;
		while (i < ARRAY_LEN(g_colors))
		{
			if (ft_subfind(sub, g_colors[i].scope, 0) < sub.length)
				return (g_colors[i].style);
			i++;
		}
		data = data->prev;
	}
	return (STYLE(0, 0, 0));
}

static void		on_token(t_spanlist *spanlist, t_parser_data *parent,
					t_sub token, void const *data)
{
	t_parser_data	color_data;
	t_style			*tmp;

	color_data = (t_parser_data){data, parent, NULL};
	parent->next = &color_data;
	tmp = ft_spanlist_push(spanlist, token.length, 1);
	*tmp = get_color(&color_data);
}

static void		refresh_syntax(t_editor *editor, t_parser const *parser)
{
	ft_spanlist_clear(&editor->spans, 1);
	exec_parser(*(t_sub*)&editor->text, parser, (t_callback[]){
		CALLBACK(on_parser_start, NULL),
		CALLBACK(on_parser_end, NULL),
		CALLBACK(on_token, &editor->spans)
	}, 0);
}

/*
** ========================================================================== **
** Init
*/

static bool		init_main(t_main *main)
{
	ft_bzero(main, sizeof(t_main));
	if (isatty(1))
	{
		if ((main->term = ft_tinit(1, TERM_RAW | TERM_LINE)) == NULL)
			return (false);
		if (main->term->flags & TERM_USE_DEFAULT)
			ft_dprintf(2, WARNING_MSG("Invalid $TERM value: Use default: %s"),
				TERM_DEFAULT_TERM);
		main->editor = NEW(t_editor);
		editor_init(main->editor);
		main->editor->user = main;
		main->flags |= FLAG_INTERACTIVE;
	}
	return (true);
}

t_parser_def const		g_parsers[] = {
	PARSER_DEF("sh", "sh",
		.tokens = PARSER_DEF_T(
			PARSER_T("\\\"", "escaped.quote"),
			PARSER_T("\\#", "escaped.comment"),
			PARSER_T("\\\'", "escaped.quote.simple"),
			PARSER_T("\\$", "escaped.dollar"),
			PARSER_T("(", "start", .parser="sh-sub"),
			PARSER_T("$(", "start", .parser="sh-sub"),
			PARSER_T("`", "start", .parser="sh-backquote"),
			PARSER_T("$((", "start", .parser="sh-math"),
			PARSER_T("${", "start", .parser="sh-expr"),
			PARSER_T(";", "op.semicolon"),
			PARSER_T("\"", "start", .parser="sh-string"),
			PARSER_T("'", "start", .parser="sh-string-simple"),
			PARSER_T("#", "start", .parser="sh-comment"),
			PARSER_T("&&", "op.and"),
			PARSER_T("&", "op.async"),
			PARSER_T("|", "op.pipe"),
			PARSER_T("||", "op.or"),
			PARSER_T("<", "redir.left"),
			PARSER_T("<<", "redir.heredoc"),
			PARSER_T(">", "redir.right"),
			PARSER_T(">>", "redir.right.double"),
			PARSER_T(" ", "space"),
			PARSER_T("\t", "space"),
			PARSER_T("\n", "space"),
		),
		.match = PARSER_DEF_T(
			PARSER_T("$?[a-zA-Z_]?*w", "var"),
			PARSER_T("$?.", "var"),
		),
	),
	PARSER_DEF("sh-sub", "sub",
		.inherit = SUBC("sh"),
		.tokens = PARSER_DEF_T(
			PARSER_T(")", "end", .end=true),
		),
	),
	PARSER_DEF("sh-backquote", "backquote",
		.inherit = SUBC("sh"),
		.tokens = PARSER_DEF_T(
			PARSER_T("`", "end", .end=true),
		),
	),
	PARSER_DEF("sh-string", "string",
		.tokens = PARSER_DEF_T(
			PARSER_T("\"", "end", .end=true),
			PARSER_T("\\\"", "escaped.quote"),
			PARSER_T("\\n", "escaped.char"),
			PARSER_T("\\e", "escaped.char"),
			PARSER_T("\\t", "escaped.char"),
			PARSER_T("`", "start", .parser="sh-backquote"),
			PARSER_T("$(", "start", .parser="sh-sub"),
			PARSER_T("$((", "start", .parser="sh-math"),
		),
		.match = PARSER_DEF_T(
			PARSER_T("$?[a-zA-Z_]?*w", "var"),
		),
	),
	PARSER_DEF("sh-string-simple", "string.simple",
		.tokens = PARSER_DEF_T(PARSER_T("'", "end", .end=true)),
	),
	PARSER_DEF("sh-comment", "comment",
		.tokens = PARSER_DEF_T(PARSER_T("\n", "endl", .end=true)),
	),
	PARSER_DEF("sh-expr", "expr",
		.tokens = PARSER_DEF_T(
			PARSER_T("}", "end", .end=true),
			PARSER_T("%", "op"),
			PARSER_T("%%", "op"),
			PARSER_T("#", "op"),
		),
	),
	PARSER_DEF("sh-math", "math",
		.inherit = SUBC("math"),
		.tokens = PARSER_DEF_T(
			PARSER_T("))", "end", .end=true),
		),
	),
	PARSER_DEF("math", "math",
		.tokens = PARSER_DEF_T(
			PARSER_T("(", "brace", .parser="math-brace"),
			PARSER_T("+", "op.plus"),
			PARSER_T("-", "op.minus"),
			PARSER_T("*", "op.mult"),
			PARSER_T("/", "op.div"),
			PARSER_T("%", "op.mod"),
			PARSER_T(" ", "space"),
			PARSER_T("\t", "space"),
			PARSER_T("\n", "space"),
		),
		.match = PARSER_DEF_T(
			PARSER_T("$?[a-zA-Z_]?*w", "var"),
			PARSER_T("?b?+d?\?(.?*d?\?'f')?b", "number"),
		),
	),
	PARSER_DEF("math-brace", "math",
		.inherit = SUBC("math"),
		.tokens = PARSER_DEF_T(
			PARSER_T(")", "close", .end=true),
		),
	),
};

static bool		init_parsers(t_main *main)
{
	main->parsers = ft_hmapnew(10, &ft_djb2);
	if (!build_parser(main->parsers, &VECTORC(g_parsers)))
		return (false);
	main->curr_parser = ft_hmapget(main->parsers, SUBC("sh")).value;
	if (main->curr_parser == NULL)
		return (false);
	return (true);
}

/*
** ========================================================================== **
** Loop
*/

static void		interactive_loop(t_main *main)
{
	t_key			key;
	uint32_t		cursor_x;
	uint32_t		cursor_y;

	ft_trestore(main->term, true);
	while (!(main->flags & FLAG_EXIT))
	{
		key = ft_getkey(0);
		ft_tclear(main->term);
		if (!editor_key(main->editor, key))
			ft_fprintf(&main->term->out, "(Unused key)%n"); // TMP
		put_key(&main->term->out, key); // TMP
		if (key.c == 'd' && key.mods == KEY_MOD_CTRL) // TMP
			main->flags |= FLAG_EXIT; // TMP
		ft_flush(&main->term->out);
		cursor_x = main->term->cursor_x;
		cursor_y = main->term->cursor_y;
		refresh_syntax(main->editor, main->curr_parser);
		editor_out(main->editor, &main->term->out);
		ft_flush(&main->term->out);
		ft_tcursor(main->term, cursor_x + main->editor->cursor, cursor_y);
	}
	ft_trestore(main->term, false);
}

static void		loop(t_main *main)
{
	t_sub			line;

	ft_dprintf(2, WARNING_MSG("Non-interactive mode"));
	while (get_next_line(0, &line) > 0)
		ft_printf("%ts%n", line); // TMP
	(void)main;
}

/*
** ========================================================================== **
** Main
*/

int				main(void)
{
	t_main			main;

	if (!init_main(&main) || !init_parsers(&main))
		return (1);
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
}
