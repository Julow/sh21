/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2015/12/17 00:42:41 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_printf.h"
#include "ft/get_next_line.h"
#include "ft/getkey.h"
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
	t_token_map const	*sh_tokens;
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

static bool		binding_test_tokenize(t_editor *editor, uint32_t flags)
{
	t_main *const	main = (t_main*)editor->user;
	t_sub const		line = *(t_sub*)&editor->text;
	t_sub			token;

	if (line.length == 0)
		return (false);
	token = SUB(line.str, 0);
	while (ft_tokenize(line, &token, NULL, main->sh_tokens))
	{
		ft_printf("Token '%ts'%n", token);
	}
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

// typedef enum s_sh_token		t_sh_token;

// enum s_sh_token
// {
// 	SH_TOKEN_ESCAPE,
// 	SH_TOKEN_AND,
// 	SH_TOKEN_AMPERSAND,
// 	SH_TOKEN_PIPE,
// 	SH_TOKEN_OR,
// 	SH_TOKEN_REDIR_RIGHT,
// 	SH_TOKEN_REDIR_RIGHT_APPEND,
// 	SH_TOKEN_REDIR_LEFT,
// 	SH_TOKEN_REDIR_HEREDOC,
// 	SH_TOKEN_DOLLAR,
// 	SH_TOKEN_QUOTE,
// 	SH_TOKEN_SINGLE_QUOTE,
// 	SH_TOKEN_PARENTHESIS_OPEN,
// 	SH_TOKEN_PARENTHESIS_CLOSE,
// 	SH_TOKEN_MATH_START,
// 	SH_TOKEN_MATH_END,
// 	SH_TOKEN_SPACE,
// 	SH_TOKEN_NL,
// };

// #define SH_TOKEN(STR,TOKEN)		{SUBC(STR), SH_TOKEN_#TOKEN, 0}
// #define SH_TOKEN_F(STR,TOKEN,F)	{SUBC(STR), SH_TOKEN_#TOKEN, F}

// struct
// {
// 	t_sub		name;
// 	t_sh_token	token;
// 	uint32_t	flags;
// } const	g_sh_tokens[] = {
// 	SH_TOKEN("\\", ESCAPE),
// 	SH_TOKEN("&&", AND),
// 	SH_TOKEN("&", AMPERSAND),
// 	SH_TOKEN("|", PIPE),
// 	SH_TOKEN("||", OR),
// 	SH_TOKEN(">", REDIR_RIGHT),
// 	SH_TOKEN(">>", REDIR_RIGHT_APPEND),
// 	SH_TOKEN("<", REDIR_LEFT),
// 	SH_TOKEN("<<", REDIR_HEREDOC),
// 	SH_TOKEN("$", DOLLAR),
// 	SH_TOKEN("(", PARENTHESIS_OPEN),
// 	SH_TOKEN(")", PARENTHESIS_CLOSE),
// 	SH_TOKEN("((", MATH_START),
// 	SH_TOKEN("))", MATH_END),
// 	SH_TOKEN("\"", QUOTE),
// 	SH_TOKEN("'", SINGLE_QUOTE),
// 	SH_TOKEN_F(" ", SPACE, TOKEN_F_REPEAT),
// 	SH_TOKEN_F("\t", SPACE, TOKEN_F_REPEAT),
// 	SH_TOKEN_F("\n", NL, TOKEN_F_REPEAT),
// };

// static void		init_sh_token(t_token_map *map)
// {
// 	uint32_t		i;

// 	i = 0;
// 	while (i < ARRAY_LEN(g_sh_tokens))
// 	{
// 		ft_token_add(map, g_sh_tokens[i].name, g_sh_tokens[i].token, 0);
// 		i++;
// 	}
// }

// {
// 	"sh": {
// 		tokens: {
// 			"\\": ESCAPE,
// 			"\"": QUOTE,
// 			"$": DOLLAR,
// 			"((": MATH_START_P,
// 			"(": PARENTHESIS_OPEN,
// 			")": PARENTHESIS_CLOSE,
// 			" ": SPACE,
// 		},
// 		end: PARENTHESIS_CLOSE,
// 		token_unmatch: {},
// 		token_merge: {
// 			[ESCAPE, QUOTE]: ESCAPED,
// 			[ESCAPE, DOLLAR]: ESCAPED,
// 			[DOLLAR, MATH_START_P]: MATH_START,
// 			[DOLLAR, TEXT]: VAR,
// 		},
// 		tokens_action: {
// 			MATH_START_P: {
// 				scope: "math",
// 				syntax: "sh-math",
// 			},
// 			PARENTHESIS_OPEN: {
// 				scope: "sub-shell",
// 				syntax: "sh",
// 			},
// 			QUOTE: {
// 				scope: "string",
// 				syntax: "sh-string",
// 			},
// 			VAR: {
// 				scope: "var"
// 			},
// 			ESCAPED: {
// 				scope: "escaped"
// 			},
// 		}
// 	}
// 	"sh-string": {
// 		tokens: {
// 			"\"": QUOTE,
// 			"\\t": ESCAPED,
// 			"\\n": ESCAPED,
// 			"\\e": ESCAPED,
// 		},
// 		end: QUOTE,
// 		token_unmatch: {},
// 		token_merge: {
// 			[ESCAPE, QUOTE]: ESCAPED,
// 		},
// 		tokens_action: {
// 			ESCAPED: {
// 				scope: "escaped"
// 			}
// 		},
// 	},
// 	"sh-math": {
// 		tokens: {
// 			"+": OPERATOR,
// 			"-": OPERATOR,
// 			"*": OPERATOR,
// 			"/": OPERATOR,
// 			"%": OPERATOR,
// 			"$": DOLLAR,
// 			" ": SPACE,
// 			"))": MATH_END_P,
// 		},
// 		end: MATH_END_P,
// 		token_unmatch: {
// 			IS_DIGIT: NUMBER,
// 		},
// 		token_merge: {
// 			[DOLLAR, UNMATCHED]: VAR
// 		},
// 		token_actions: {
// 			OPERATOR: {
// 				scope: "op"
// 			},
// 			VAR: {
// 				scope: "var"
// 			}
// 		}
// 	}
// }

#define T(STR)		{SUBC(STR), 0}

static t_token_def	g_sh_tokens[] = {
	T("\\"),
	T("&&"),
	T("&"),
	T("|"),
	T("||"),
	T(">"),
	T(">>"),
	T("<"),
	T("<<"),
	T("$"),
	T("("),
	T(")"),
	T("(("),
	T("))"),
	T("\""),
	T("'"),
	T(" "),
	T("\t"),
	T("\n"),
};

#undef T

t_token_map const	*init_sh_tokens(void)
{
	t_token_map *const	token_map = MAL1(t_token_map);

	ft_token_map(token_map, g_sh_tokens, ARRAY_LEN(g_sh_tokens));
	return (token_map);
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

	if (!init_main(&main))
		return (0);
	main.sh_tokens = init_sh_tokens();
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
}
