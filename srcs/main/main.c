/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/01/19 16:15:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_hmap.h"
#include "ft/ft_printf.h"
#include "ft/get_next_line.h"
#include "ft/getkey.h"
#include "ft/term.h"
#include "ft/tokenizer.h"

#include "editor.h"
#include "syntax.h"
#include "syntax_def.h"

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
	t_hmap				*syntaxes;
	t_syntax			*curr_syntax;
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

static bool		test_binding(t_editor *editor, uint32_t flags)
{
	uint32_t const	c = flags >> 16;
	uint32_t const	len = flags & 0xFFFF;

	ft_memset(ft_dstrspan(&editor->text, editor->cursor,
		editor->cursor + editor->sel, len), c, len);
	if (editor->sel < 0)
		editor->cursor += editor->sel;
	editor->cursor += len;
	editor->sel = 0;
	return (true);
}

/*
** ========================================================================== **
** Debug syntax
*/
// mdr (truc "loool''" 'xd" $LOL $(mdr)' $(( 5 + 4/(8.0-3.f))) "$(xd "mdr$A" '$lol')$LOL") lol

/*
** Check if 'str' match 'pattern'
** Pattern syntax:
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

#define SCOPE(S,C)	{SUBC(S), SUBC(C)}

static struct {
	t_sub			scope;
	t_sub			color;
} const			g_colors[] = {
	SCOPE("string.simple", C_YELLOW),
	SCOPE("string", C_LYELLOW),
	SCOPE("escaped", C_LRED),
	SCOPE("math", BG_LGREEN),
	SCOPE("comment", C_BLUE),
	SCOPE("start", C_WHITE),
	SCOPE("end", C_WHITE),
};

static uint32_t	scope_match(t_sub scope, t_sub match)
{
	uint32_t		score;
	t_sub			s;
	t_sub			tmp;

	score = 0;
	s = SUB_START(scope);
	while (ft_subnext_c(scope, &s, '.'))
	{
		tmp = SUB_START(match);
		while (ft_subnext_c(match, &tmp, '.'))
		{
			if (SUB_EQU(s, tmp))
			{
				match = SUB_FOR(match, tmp.str - match.str);
				score++;
				break ;
			}
		}
	}
	return (score);
}

static void		token_callback(t_sub token, t_syntax_data *data, void *env)
{
	ft_printf("'%ts' " C_GRAY, token);
	while (data != NULL)
	{
		ft_printf(".%s", data->data);
		data = data->prev;
	}
	ft_printf(C_RESET "%n");
	(void)env;
}

static bool		binding_test_tokenize(t_editor *editor, uint32_t flags)
{
	t_main *const	main = (t_main*)editor->user;
	t_sub const		line = *(t_sub*)&editor->text;

	exec_syntax(line, &token_callback, main->curr_syntax, NULL);
	ft_printf("%n");
	return (true);
	(void)flags;
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
		editor_bind(main->editor, KEY('C', KEY_MOD_SHIFT), &test_binding, ('a' << 16) | 10); // tmp
		editor_bind(main->editor, KEY('m', KEY_MOD_CTRL), &binding_test_tokenize, 0); // tmp
		main->editor->user = main;
		main->flags |= FLAG_INTERACTIVE;
	}
	return (true);
}

t_syntax_def const		g_syntaxes[] = {
	SYNTAX_DEF("sh", "sh",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\\\"", "escaped.quote"),
			SYNTAX_T("\\#", "escaped.comment"),
			SYNTAX_T("\\\'", "escaped.quote.simple"),
			SYNTAX_T("\\$", "escaped.dollar"),
			SYNTAX_T("(", "start", .syntax="sh-sub"),
			SYNTAX_T("$(", "start", .syntax="sh-sub"),
			SYNTAX_T("`", "start", .syntax="sh-backquote"),
			SYNTAX_T("$((", "start", .syntax="sh-math"),
			SYNTAX_T("${", "start", .syntax="sh-expr"),
			SYNTAX_T(";", "op.semicolon"),
			SYNTAX_T("\"", "start", .syntax="sh-string"),
			SYNTAX_T("'", "start", .syntax="sh-string-simple"),
			SYNTAX_T("#", "start", .syntax="sh-comment"),
			SYNTAX_T("&&", "op.and"),
			SYNTAX_T("&", "op.async"),
			SYNTAX_T("|", "op.pipe"),
			SYNTAX_T("||", "op.or"),
			SYNTAX_T("<", "redir.left"),
			SYNTAX_T("<<", "redir.heredoc"),
			SYNTAX_T(">", "redir.right"),
			SYNTAX_T(">>", "redir.right.double"),
			SYNTAX_T(" ", "space"),
			SYNTAX_T("\t", "space"),
			SYNTAX_T("\n", "space"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
			SYNTAX_T("$?.", "var"),
		),
	),
	SYNTAX_DEF("sh-sub", "sub",
		.inherit = SUBC("sh"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(")", "end", .end=true),
		),
	),
	SYNTAX_DEF("sh-backquote", "backquote",
		.inherit = SUBC("sh"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("`", "end", .end=true),
		),
	),
	SYNTAX_DEF("sh-string", "string",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\"", "end", .end=true),
			SYNTAX_T("\\\"", "escaped.quote"),
			SYNTAX_T("\\n", "escaped.char"),
			SYNTAX_T("\\e", "escaped.char"),
			SYNTAX_T("\\t", "escaped.char"),
			SYNTAX_T("`", "start", .syntax="sh-backquote"),
			SYNTAX_T("$(", "start", .syntax="sh-sub"),
			SYNTAX_T("$((", "start", .syntax="sh-math"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
		),
	),
	SYNTAX_DEF("sh-string-simple", "string.simple",
		.tokens = SYNTAX_DEF_T(SYNTAX_T("'", "end", .end=true)),
	),
	SYNTAX_DEF("sh-comment", "comment",
		.tokens = SYNTAX_DEF_T(SYNTAX_T("\n", "endl", .end=true)),
	),
	SYNTAX_DEF("sh-expr", "expr",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("}", "end", .end=true),
			SYNTAX_T("%", "op"),
			SYNTAX_T("%%", "op"),
			SYNTAX_T("#", "op"),
		),
	),
	SYNTAX_DEF("sh-math", "math",
		.inherit = SUBC("math"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("))", "end", .end=true),
		),
	),
	SYNTAX_DEF("math", "math",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("(", "brace", .syntax="math-brace"),
			SYNTAX_T("+", "op.plus"),
			SYNTAX_T("-", "op.minus"),
			SYNTAX_T("*", "op.mult"),
			SYNTAX_T("/", "op.div"),
			SYNTAX_T("%", "op.mod"),
			SYNTAX_T(" ", "space"),
			SYNTAX_T("\t", "space"),
			SYNTAX_T("\n", "space"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
			SYNTAX_T("?b?+d?\?(.?*d?\?'f')?b", "number"),
		),
	),
	SYNTAX_DEF("math-brace", "math",
		.inherit = SUBC("math"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(")", "close", .end=true),
		),
	),
};

static bool		init_syntaxes(t_main *main)
{
	main->syntaxes = ft_hmapnew(10, &ft_djb2);
	if (!build_syntax(main->syntaxes, &VECTORC(g_syntaxes)))
		return (false);
	main->curr_syntax = ft_hmapget(main->syntaxes, SUBC("sh")).value;
	if (main->curr_syntax == NULL)
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
		editor_put(main->editor, &main->term->out);
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

	if (!init_main(&main) || !init_syntaxes(&main))
		return (1);
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
}
