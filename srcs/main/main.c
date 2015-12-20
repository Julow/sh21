/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2015/12/20 00:12:53 by juloo            ###   ########.fr       */
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

static void		token_callback(t_sub token, t_sub scope, void *env)
{
	ft_printf("TOKEN '%ts' " C_GRAY "%ts" C_RESET "%n", token, scope);
	(void)env;
}

static bool		binding_test_tokenize(t_editor *editor, uint32_t flags)
{
	t_main *const	main = (t_main*)editor->user;
	t_sub const		line = *(t_sub*)&editor->text;

	exec_syntax(line, &token_callback, main->curr_syntax, NULL);
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
		main->editor = MAL1(t_editor);
		editor_init(main->editor);
		editor_bind(main->editor, KEY('C', KEY_MOD_SHIFT), &test_binding, ('a' << 16) | 10); // tmp
		editor_bind(main->editor, KEY('m', KEY_MOD_CTRL), &binding_test_tokenize, 0); // tmp
		main->editor->user = main;
		main->flags |= FLAG_INTERACTIVE;
	}
	return (true);
}

t_syntax_def const		g_syntaxes[] = {
	SYNTAX_DEF("sh", "sh", "",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\\\"", "escaped.quote"),
			SYNTAX_T("\\\'", "escaped.quote.simple"),
			SYNTAX_T("\\$", "escaped.dollar"),
			SYNTAX_T("(", "start", "sh-sub"),
			SYNTAX_T("$(", "start", "sh-sub"),
			SYNTAX_T("`", "start", "sh-backquote"),
			SYNTAX_T("$((", "start", "sh-math"),
			SYNTAX_T("${", "start", "sh-expr"),
			SYNTAX_T(";", "op.semicolon"),
			SYNTAX_T("\"", "start", "sh-string"),
			SYNTAX_T("'", "start", "sh-string-simple"),
			SYNTAX_T("#", "start", "sh-comment"),
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
		// .match = (t_syntax_def_t[]){
		// 	{"$[a-zA-Z_][a-zA-Z0-9_]*", "var"},
		// 	{"if", "if", "sh-if"},
		// 	{"then", "keyword.then"},
		// 	{"do", "keyword.do"},
		// 	{"done", "keyword.done"},
		// 	{"fi", "keyword.fi"},
		// 	{"else", "keyword.else"},
		// 	{"elif", "keyword.elif"},
		// 	{"while", "keyword.while"},
		// 	{"until", "keyword.until"},
		// 	{"for", "keyword.for"},
		// 	{"in", "keyword.in"},
		// },
	),
	SYNTAX_DEF("sh-sub", "sub", "end",
		.inherit = SUBC("sh"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(")", "end"),
		),
	),
	SYNTAX_DEF("sh-backquote", "backquote", "end",
		.inherit = SUBC("sh"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("`", "end"),
		),
	),
	// {"sh-if", ";",
	// 	.inherit = "sh",
	// 	.tokens = (t_syntax_def_t[]){
	// 		{"[", "op.test", "sh-test"},
	// 		{"[[", "op.test.double", "sh-test-double"},
	// 	},
	// },
	// {"sh-test", "]",
	// 	.inherit = "sh",
	// 	.tokens = (t_syntax_def_t[]){
	// 		{"=", "op"}
	// 		{"==", "op"}
	// 		{"!=", "op"}
	// 		{"<", "op"}
	// 		{">", "op"}
	// 	},
	// 	.match = (t_syntax_def_f[]){
	// 		{"-[a-zA-Z]{1,2}", "test"},
	// 	},
	// },
	// {"sh-test-double", "]]", .inherit = "sh-test"},
	SYNTAX_DEF("sh-string", "string", "end",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\"", "end"),
			SYNTAX_T("\\\"", "escaped.quote"),
			SYNTAX_T("\\n", "escaped.char"),
			SYNTAX_T("\\e", "escaped.char"),
			SYNTAX_T("\\t", "escaped.char"),
			SYNTAX_T("`", "start", "sh-backquote"),
			SYNTAX_T("$(", "start", "sh"),
			SYNTAX_T("$((", "start", "sh-math"),
		),
	),
	SYNTAX_DEF("sh-string-simple", "string.simple", "end",
		.tokens = SYNTAX_DEF_T(SYNTAX_T("'", "end")),
	),
	SYNTAX_DEF("sh-comment", "comment", "endl",
		.tokens = SYNTAX_DEF_T(SYNTAX_T("\n", "endl")),
	),
	SYNTAX_DEF("sh-expr", "expr", "end",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("}", "end"),
			SYNTAX_T("%", "op"),
			SYNTAX_T("%%", "op"),
			SYNTAX_T("#", "op"),
		),
	),
	SYNTAX_DEF("sh-math", "math", "end",
		.inherit = SUBC("math"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("))", "end"),
		),
	),
	SYNTAX_DEF("math", "math", "",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("(", "brace", "math-brace"),
			SYNTAX_T("+", "op.plus"),
			SYNTAX_T("-", "op.minus"),
			SYNTAX_T("*", "op.mult"),
			SYNTAX_T("/", "op.div"),
			SYNTAX_T("%", "op.mod"),
			SYNTAX_T(" ", "space"),
			SYNTAX_T("\t", "space"),
			SYNTAX_T("\n", "space"),
		),
		// .match = (t_syntax_def_t[]){
		// 	{"$[a-zA-Z_][a-zA-Z0-9_]*", "var"},
		// 	{"[0-9]+\\.?[0-9]*", "number"},
		// },
	),
	SYNTAX_DEF("math-brace", "math", "close",
		.inherit = SUBC("math"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(")", "close"),
		),
	),
};

static bool		init_syntaxes(t_main *main)
{
	main->syntaxes = ft_hmapnew(10, &ft_djb2);
	if (!build_syntax(main->syntaxes, &VECTORC(g_syntaxes)))
		return (false);
	if ((main->curr_syntax
		= ft_hmapget(main->syntaxes, SUBC("sh")).value) == NULL)
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
