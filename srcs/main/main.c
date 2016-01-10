/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/01/10 00:27:26 by juloo            ###   ########.fr       */
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
static uint32_t	match_str(t_sub str, t_sub pattern);

static uint32_t	match_wildcard(t_sub str, t_sub pattern, uint32_t i)
{
	uint32_t		tmp;
	uint32_t		max;

	tmp = i;
	while (++tmp < pattern.length)
		if (pattern.str[tmp] == '?')
			i++;
		else if (!(pattern.str[tmp] == '*'))
			break ;
	if (tmp >= pattern.length)
		return (str.length);
	pattern = SUB(pattern.str + tmp, pattern.length - tmp);
	max = 0;
	while (i < str.length)
	{
		if (str.str[i] == pattern.str[0])
		{
			tmp = match_str(SUB(str.str + i, str.length - i), pattern) + i;
			if (tmp > i && tmp > max)
				max = tmp;
		}
		i++;
	}
	return (max);
}

static uint32_t	match_str(t_sub str, t_sub pattern)
{
	uint32_t		len;
	uint32_t		i;

	i = 0;
	len = MIN(str.length, pattern.length);
	while (i < len)
	{
		if (pattern.str[i] == '*')
			return (match_wildcard(str, pattern, i));
		if (str.str[i] != pattern.str[i] && pattern.str[i] != '?')
			return (0);
		i++;
	}
	while (i < pattern.length && pattern.str[i] == '*')
		pattern = SUB(pattern.str + 1, pattern.length - 1);
	return ((i == pattern.length) ? i : 0);
}

bool			ft_match(t_sub str, t_sub *match, t_sub pattern)
{
	uint32_t		i;
	uint32_t		tmp;

	if (match == NULL)
		return (BOOL_OF(match_str(str, pattern) == str.length));
	i = match->str + match->length - str.str;
	tmp = 0;
	while (i < str.length)
	{
		tmp = match_str(SUB(str.str + i, str.length - i), pattern);
		if (tmp > 0)
			break ;
		i++;
	}
	match->str = str.str + i;
	match->length = tmp;
	return (BOOL_OF(tmp > 0));
}

static void		token_callback(t_sub token, t_sub scope, void *env)
{
	char const		*color;
	// uint32_t		i;
	// uint32_t		count;

	// count = 0;
	// i = 0;
	// while (i < scope.length)
	// {
	// 	if (scope.str[i] == '.')
	// 		count++;
	// 	i++;
	// }
	// ft_printf("%.*c'%ts' " C_GRAY "%ts" C_RESET "%n",
	// 	count * 4 - 3, ' ', token, scope);
	ft_printf(C_GRAY "%ts" C_RESET " '%ts'%n", scope, token);
	// if (ft_match(scope, SUBC("*.escaped:*")) || ft_match(scope, SUBC("*.var")))
	// 	color = C_BLUE;
	// else if (ft_match(scope, SUBC("*.string:simple.*")))
	// 	color = C_YELLOW;
	// else if (ft_match(scope, SUBC("*.math.*")))
	// 	color = BG_BLUE;
	// else if (ft_match(scope, SUBC("*.string.*")))
	// 	color = C_RED;
	// else
	// 	color = "";
	// ft_printf("%s%ts" C_RESET BG_RESET, color, token);
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
	// SYNTAX_DEF("sh", "sh", "",
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("\\\"", "escaped:quote"),
	// 		SYNTAX_T("\\\'", "escaped:quote:simple"),
	// 		SYNTAX_T("\\$", "escaped:dollar"),
	// 		SYNTAX_T("(", "start", "sh-sub"),
	// 		SYNTAX_T("$(", "start", "sh-sub"),
	// 		SYNTAX_T("`", "start", "sh-backquote"),
	// 		SYNTAX_T("$((", "start", "sh-math"),
	// 		SYNTAX_T("${", "start", "sh-expr"),
	// 		SYNTAX_T(";", "op:semicolon"),
	// 		SYNTAX_T("\"", "start", "sh-string"),
	// 		SYNTAX_T("'", "start", "sh-string-simple"),
	// 		SYNTAX_T("#", "start", "sh-comment"),
	// 		SYNTAX_T("&&", "op:and"),
	// 		SYNTAX_T("&", "op:async"),
	// 		SYNTAX_T("|", "op:pipe"),
	// 		SYNTAX_T("||", "op:or"),
	// 		SYNTAX_T("<", "redir:left"),
	// 		SYNTAX_T("<<", "redir:heredoc"),
	// 		SYNTAX_T(">", "redir:right"),
	// 		SYNTAX_T(">>", "redir:right.double"),
	// 		SYNTAX_T(" ", "space"),
	// 		SYNTAX_T("\t", "space"),
	// 		SYNTAX_T("\n", "space"),
	// 	),
	// 	.match = SYNTAX_DEF_T(
	// 		SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
	// 		SYNTAX_T("$?.", "var"),
	// 	),
	// ),
	// SYNTAX_DEF("sh-sub", "sub", "end",
	// 	.inherit = SUBC("sh"),
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T(")", "end"),
	// 	),
	// ),
	// SYNTAX_DEF("sh-backquote", "backquote", "end",
	// 	.inherit = SUBC("sh"),
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("`", "end"),
	// 	),
	// ),
	// SYNTAX_DEF("sh-string", "string", "end",
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("\"", "end"),
	// 		SYNTAX_T("\\\"", "escaped:quote"),
	// 		SYNTAX_T("\\n", "escaped:char"),
	// 		SYNTAX_T("\\e", "escaped:char"),
	// 		SYNTAX_T("\\t", "escaped:char"),
	// 		SYNTAX_T("`", "start", "sh-backquote"),
	// 		SYNTAX_T("$(", "start", "sh-sub"),
	// 		SYNTAX_T("$((", "start", "sh-math"),
	// 	),
	// 	.match = SYNTAX_DEF_T(
	// 		SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
	// 	),
	// ),
	// SYNTAX_DEF("sh-string-simple", "string:simple", "end",
	// 	.tokens = SYNTAX_DEF_T(SYNTAX_T("'", "end")),
	// ),
	// SYNTAX_DEF("sh-comment", "comment", "endl",
	// 	.tokens = SYNTAX_DEF_T(SYNTAX_T("\n", "endl")),
	// ),
	// SYNTAX_DEF("sh-expr", "expr", "end",
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("}", "end"),
	// 		SYNTAX_T("%", "op"),
	// 		SYNTAX_T("%%", "op"),
	// 		SYNTAX_T("#", "op"),
	// 	),
	// ),
	// SYNTAX_DEF("sh-math", "math", "end",
	// 	.inherit = SUBC("math"),
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("))", "end"),
	// 	),
	// ),
	// SYNTAX_DEF("math", "math", "",
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T("(", "brace", "math-brace"),
	// 		SYNTAX_T("+", "op:plus"),
	// 		SYNTAX_T("-", "op:minus"),
	// 		SYNTAX_T("*", "op:mult"),
	// 		SYNTAX_T("/", "op:div"),
	// 		SYNTAX_T("%", "op:mod"),
	// 		SYNTAX_T(" ", "space"),
	// 		SYNTAX_T("\t", "space"),
	// 		SYNTAX_T("\n", "space"),
	// 	),
	// 	.match = SYNTAX_DEF_T(
	// 		SYNTAX_T("$?[a-zA-Z_]?*w", "var"),
	// 		SYNTAX_T("?b?+d?\?(.?*d?\?'f')?b", "number"),
	// 	),
	// ),
	// SYNTAX_DEF("math-brace", "math", "close",
	// 	.inherit = SUBC("math"),
	// 	.tokens = SYNTAX_DEF_T(
	// 		SYNTAX_T(")", "close"),
	// 	),
	// ),
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
** TEST
*/

t_syntax_def const		g_test_syntax[] = {
	SYNTAX_DEF("c", "c",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(";", "semicolon"),
			SYNTAX_T("//", "begin", .syntax="c-comment"),
			SYNTAX_T("/*", "begin", .syntax="c-comment:block"),
			SYNTAX_T("\"", "quote", .syntax="c-string"),
			SYNTAX_T("'", "quote", .syntax="string-simple"),
			SYNTAX_T("#", "begin", .syntax="c-preprocessor"),
			SYNTAX_T("(", "begin", .syntax="c-parenthesis"),
			SYNTAX_T("{", "begin", .syntax="c-block"),
			SYNTAX_T(" ", "space"),
			SYNTAX_T("\t", "space"),
			SYNTAX_T("\n", "space"),
			SYNTAX_T(",", "comma"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("?b?'return'|'while'|'for'|'if'|'else'|'do'|'break'|'continue'|'struct'|'typedef'?b", "keyword"),
			SYNTAX_T("?b?+d??(.?*d)??[f]?b", "constant.number"),
			SYNTAX_T("?b?'true'|'false'|'NULL'?b", "constant"),
			SYNTAX_T("?b?(??'u'int??'_fast'?'8'|'16'|'32'|'64'_t)|'int'|'unsigned'|'long'|'char'|'void'|'byte'|'short'|'bool'|'const'|(t_?+w)|(?+w_t)?b", "type"),
		),
	),
	SYNTAX_DEF("c-parenthesis", "parenthesis",
		.inherit=SUBC("c"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T(")", "end", .end=true),
		),
	),
	SYNTAX_DEF("c-block", "block",
		.inherit=SUBC("c"),
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("}", "end", .end=true),
		),
	),
	SYNTAX_DEF("c-comment:block", "comment.block",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("*/", "end", .end=true),
		),
	),
	SYNTAX_DEF("c-comment", "comment",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\n", "newline", .end=true),
		),
	),
	SYNTAX_DEF("c-preprocessor", "preprocessor",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\n", "newline", .end=true),
			SYNTAX_T("\\\n", "nl"),
			SYNTAX_T(" ", "space"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("?binclude?b", "directive", .syntax="c-preprocessor-include", .end=true),
		),
	),
	SYNTAX_DEF("c-preprocessor-include", "include",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\n", "newline", .end=true),
			SYNTAX_T("\"", "quote", .syntax="c-string"),
			SYNTAX_T("<", "angle", .syntax="string-angle"),
		),
	),
	SYNTAX_DEF("c-string", "string",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\\\"", "escaped.quote"),
			SYNTAX_T("\"", "quote", .end=true),
			SYNTAX_T("\\r", "escaped.r"),
			SYNTAX_T("\\n", "escaped.n"),
			SYNTAX_T("\\t", "escaped.t"),
			SYNTAX_T("\\\\?", "escaped.?"),
			SYNTAX_T("\\\\", "escaped.escape"),
		),
		.match = SYNTAX_DEF_T(
			SYNTAX_T("%?*[^Mm+ '#-]?'*'|*d?\?(.?+d|'*')?'hh'|'ll'|?[jtzqhlL]?[sSdDoOuUxXicCnpfFeE%]", "format.printf"),
			SYNTAX_T("?'?'??(#??'#'{?-+.})?*[!i=-]?(?*d,?*d)|?[?*+]??(&?*d)?[.aludnswb^$]|('?-*.')|(\"?-*.\")|([?-*.])|({??[&:]?-*.})|'('", "format.regex"),
			SYNTAX_T("?[\\]?'033'|'e'[?+dm", "format.color"),
		),
	),
	SYNTAX_DEF("string-angle", "string",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\\>", "escaped.quote"),
			SYNTAX_T(">", "angle.end", .end=true),
		),
	),
	SYNTAX_DEF("string-simple", "string",
		.tokens = SYNTAX_DEF_T(
			SYNTAX_T("\\'", "escaped.quote"),
			SYNTAX_T("'", "quote", .end=true),
		),
	),
};

static void		test_token_callback(t_sub token, t_sub scope, void *env)
{
	// ft_printf("TOKEN '%ts' '%ts'%n", token, scope);
	char const		*color;

	color = "";
	if (ft_match(scope, NULL, SUBC("*.error.*")))
		color = BG_RED;
	else if (ft_match(scope, NULL, SUBC("*.begin*")))
		color = BG_GREEN;
	else if (ft_match(scope, NULL, SUBC("*.end*")))
		color = BG_GREEN;
	else if (ft_match(scope, NULL, SUBC("*.keyword*")))
		color = C_GREEN;
	else if (ft_match(scope, NULL, SUBC("*.type*")))
		color = C_CYAN;
	else if (ft_match(scope, NULL, SUBC("*.constant*")))
		color = C_RED;
	else if (ft_match(scope, NULL, SUBC("*.comment.*")))
		color = C_BLUE;
	else if (ft_match(scope, NULL, SUBC("*.format.*")))
		color = C_LYELLOW;
	else if (ft_match(scope, NULL, SUBC("*.escaped.*")))
		color = C_LRED;
	else if (ft_match(scope, NULL, SUBC("*.string.*")))
		color = C_YELLOW;
	else if (ft_match(scope, NULL, SUBC("*.preprocessor.*")))
		color = C_RED;
	ft_printf("%s%ts" C_RESET BG_RESET, color, token);
}
#include <fcntl.h>
#include <unistd.h>

void			test(char const *file)
{
	t_hmap *const	syntax_map = ft_hmapnew(50, &ft_djb2);

	if (!build_syntax(syntax_map, &VECTORC(g_test_syntax)))
		return ;

	t_syntax		*syntax = ft_hmapget(syntax_map, SUBC("c")).value;

	int const		fd = open(file, O_RDONLY);
	t_dstr			code;
	char const		buff[512];
	int32_t			len;

	if (fd < 0)
		return ;
	code = DSTR0();
	while ((len = read(fd, buff, 512)) > 0)
		ft_dstradd(&code, SUB(buff, len));
	exec_syntax(SUB(code.str, code.length), &test_token_callback, syntax, NULL);
	ft_printf("%n");
}

/*
** ========================================================================== **
** Main
*/

static void		test_match(t_sub str, t_sub pattern)
{
	t_sub			match;

	ft_printf("Match '%ts' : '%ts'%n", str, pattern);
	ft_printf("Strict match: %s%n", ft_match(str, NULL, pattern) ? "true" : "false");
	match = SUB(str.str, 0);
	ft_printf("Iterative match:");
	while (ft_match(str, &match, pattern))
		ft_printf(" '%ts'", match);
	ft_printf("%n");
}

int				main(int argc, char **argv)
{
	if (argc > 2)
		return (test_match(ft_sub(argv[1], 0, -1), ft_sub(argv[2], 0, -1)), 0);
	if (argc > 1)
		return (test(argv[1]), 0);
	t_main			main;

	if (!init_main(&main) || !init_syntaxes(&main))
		return (1);
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
}
