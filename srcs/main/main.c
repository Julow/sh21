/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/02/10 17:54:19 by jaguillo         ###   ########.fr       */
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
#include "sh_cmd.h"
#include "syntax_color.h"

#include <termios.h>
#include <unistd.h>

#include <sys/select.h>

/*
** TODO: parser: multi inherit
** TODO: parser: .tail=true to change parser without recursion
** TODO: editor: multi cursor
** TODO: editor: undo/redo history
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
	t_syntax_color const	*syntax_color;
};

#define DEFAULT_SYNTAX_COLOR	"sh"

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

	if (key.mods & MOD_CTRL)
		ft_putsub(out, SUBC("ctrl+"));
	if (key.mods & MOD_ALT)
		ft_putsub(out, SUBC("alt+"));
	if (key.mods & MOD_SHIFT)
		ft_putsub(out, SUBC("shift+"));
	if (IS(key.c, IS_PRINT))
		ft_fprintf(out, "%c", key.c);
	else if ((key_name = get_key_name(key)) != NULL)
		ft_fprintf(out, "%s", key_name);
	else
		ft_fprintf(out, "0x%0.2x", (uint32_t)key.c);
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

#define SCOPE(S,C)	{SUBC(S), &STYLE C}

static t_vector const	g_color_scheme = VECTOR(t_color_scheme,
	SCOPE("string.simple", (S_YELLOW, 0, 0)),
	SCOPE("string", (S_LIGHT(S_YELLOW), 0, 0)),
	SCOPE("escaped", (S_LIGHT(S_RED), 0, 0)),
	SCOPE("number", (S_YELLOW, 0, 0)),
	SCOPE("op", (S_LIGHT(S_WHITE), 0, 0)),
	SCOPE("subst", (S_CYAN, 0, 0)),
	SCOPE("comment", (0, S_BLUE, 0)),
	SCOPE("identifier.key", (S_LIGHT(S_CYAN), 0, 0)),
	SCOPE("identifier", (S_GREEN, 0, 0)),
	SCOPE("keyword", (S_MAGENTA, 0, 0)),
	SCOPE("error", (0, S_RED, 0)),
);

static void		sh_spanlist_fill(t_spanlist *spanlist,
					t_vec2u range, t_style const *style)
{
	t_style			*tmp;

	tmp = ft_spanlist_push(spanlist, range.y - range.x, 1);
	*tmp = (style == NULL) ? STYLE(0, 0, 0) : *style;
}

static void		refresh_syntax(t_editor *editor, t_syntax_color const *s)
{
	t_in			parse_in;

	parse_in = IN(editor->text.str, editor->text.length, NULL);
	ft_spanlist_clear(&editor->spans, 1);
	exec_syntax_color(&parse_in, s, &g_color_scheme,
		CALLBACK(sh_spanlist_fill, &editor->spans));
}

/*
** ========================================================================== **
** Exec shell
*/

/*
** ========================================================================== **
** Shell parser
*/

enum		e_sh_token
{
	SH_T_AND = 1,
	SH_T_OR,
	SH_T_PIPE,
	SH_T_SEMICOLON,
	SH_T_AMPERSAND,
	SH_T_NEWLINE,
//
	SH_T_SPACE,
	SH_T_BEGIN,
	SH_T_END,
	SH_T_ESCAPED,
//
	SH_T_SUBST_PARAM,
	SH_T_SUBST_PARAM_SPECIAL,
};

enum		e_sh_parser
{
	SH_P_SHELL = 1,
	SH_P_SUBSHELL,
	SH_P_BACKQUOTE,
	SH_P_EXPR,
	SH_P_MATH,
	SH_P_STRING,
	SH_P_STRING_SINGLE,
	SH_P_IGNORE,
};

#define T(STR,T,...)	PARSER_T(STR, V(SH_T_##T), ##__VA_ARGS__)

static bool		sh_parse_cmd(t_parse_data *p);
static bool		sh_parse_sub(t_parse_data *p);
// static bool		sh_parse_expr(t_parse_data *p);
// static bool		sh_parse_math(t_parse_data *p);
static bool		sh_parse_string(t_parse_data *p);

t_parser_def const	g_sh_parser[] = {

	PARSER_DEF("sh-base-subst", NULL, NULL,
		.tokens = PARSER_DEF_T(
			T("${", BEGIN, .parser="sh-expr"),
			T("$(", BEGIN, .parser="sh-sub"),
			T("$((", BEGIN, .parser="sh-math"),
			T("`", BEGIN, .parser="sh-backquote"),
		),
		.match = PARSER_DEF_T(
			T("$?[a-zA-Z_]?*w", SUBST_PARAM),
			T("$?.", SUBST_PARAM_SPECIAL),
		),
	),

	PARSER_DEF("sh-cmd", V(SH_P_SHELL), &sh_parse_cmd,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("&&", AND, .end=true, .parser="sh-cmd"),
			T("||", OR, .end=true, .parser="sh-cmd"),
			T("|", PIPE, .end=true, .parser="sh-cmd"),
			T(";", SEMICOLON, .end=true),
			T("&", AMPERSAND, .end=true),
			T("\n", NEWLINE, .end=true),
			T(" ", SPACE),
			T("\t", SPACE),
			T("\"", BEGIN, .parser="sh-string"),
			T("'", BEGIN, .parser="sh-string-single"),
			T("#", BEGIN, .parser="sh-comment"),

			T("\\ ", ESCAPED),
			T("\\\t", ESCAPED),
			T("\\\n", ESCAPED),
			T("\\;", ESCAPED),
			T("\\|", ESCAPED),
			T("\\&", ESCAPED),
			T("\\`", ESCAPED),
			T("\\\"", ESCAPED),
			T("\\'", ESCAPED),

		),
	),

	PARSER_DEF("sh-sub", V(SH_P_SUBSHELL), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T(")", END, .end=true),
		),
	),

	PARSER_DEF("sh-backquote", V(SH_P_BACKQUOTE), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("`", END, .end=true),
			T("\\`", BEGIN, .parser="sh-backquote-rev"),
		),
	),

	PARSER_DEF("sh-backquote-rev", V(SH_P_BACKQUOTE), &sh_parse_sub,
		PARSER_INHERIT("sh-cmd"),
		.tokens = PARSER_DEF_T(
			T("\\`", END, .end=true),
			T("`", BEGIN, .parser="sh-backquote"),
		),
	),

	PARSER_DEF("sh-expr", V(SH_P_EXPR), NULL,
		.tokens = PARSER_DEF_T(
			T("}", END, .end=true),
		),
	),

	PARSER_DEF("sh-math", V(SH_P_MATH), NULL,
		.tokens = PARSER_DEF_T(
			T("))", END, .end=true),
		),
	),

	PARSER_DEF("sh-string", V(SH_P_STRING), &sh_parse_string,
		PARSER_INHERIT("sh-base-subst"),
		.tokens = PARSER_DEF_T(
			T("\"", END, .end=true),
			T("\\\"", ESCAPED),
		),
	),

	PARSER_DEF("sh-string-single", V(SH_P_STRING_SINGLE), &sh_parse_string,
		.tokens = PARSER_DEF_T(
			T("'", END, .end=true),
		),
	),

	PARSER_DEF("sh-comment", V(SH_P_IGNORE), NULL,
		.tokens = PARSER_DEF_T(
			T("\n", END, .end=true),
		),
	),

};

#undef T

static t_parser const	*load_sh_parser(void)
{
	t_hmap					*map;
	t_parser const			*parser;

	map = ft_hmapnew(10, &ft_djb2);
	if (!build_parser(map, &VECTORC(g_sh_parser)))
		return (NULL);
	parser = ft_hmapget(map, SUBC("sh-cmd")).value;
	// ft_hmapdestroy(map, NULL);
	return (parser);
}

t_sub const		g_sh_token_str[] = {
	[0] = SUBC("NULL"),
	[SH_T_AND] = SUBC("AND"),
	[SH_T_OR] = SUBC("OR"),
	[SH_T_PIPE] = SUBC("PIPE"),
	[SH_T_SEMICOLON] = SUBC("SEMICOLON"),
	[SH_T_AMPERSAND] = SUBC("AMPERSAND"),
	[SH_T_NEWLINE] = SUBC("NEWLINE"),
	[SH_T_SPACE] = SUBC("SPACE"),
	[SH_T_BEGIN] = SUBC("BEGIN"),
	[SH_T_END] = SUBC("END"),
	[SH_T_ESCAPED] = SUBC("ESCAPED"),
	[SH_T_SUBST_PARAM] = SUBC("SUBST_PARAM"),
	[SH_T_SUBST_PARAM_SPECIAL] = SUBC("SUBST_PARAM_SPECIAL"),
};

t_sub const		g_sh_parser_str[] = {
	[0] = SUBC("NULL"),
	[SH_P_SHELL] = SUBC("SHELL"),
	[SH_P_SUBSHELL] = SUBC("SUBSHELL"),
	[SH_P_BACKQUOTE] = SUBC("BACKQUOTE"),
	[SH_P_EXPR] = SUBC("EXPR"),
	[SH_P_MATH] = SUBC("MATH"),
	[SH_P_STRING] = SUBC("STRING"),
	[SH_P_STRING_SINGLE] = SUBC("STRING_SINGLE"),
	[SH_P_IGNORE] = SUBC("IGNORE"),
};

static bool		sh_parse_cmd(t_parse_data *p)
{
	ft_printf("BEGIN CMD%n");
	while (parse_token(p))
	{
		if ((uintptr_t)p->token_data > SH_T_SUBST_PARAM_SPECIAL)
			ft_printf("  TOKEN '%ts' <INVALID> %lld%n", p->token, p->token_data);
		else
			ft_printf("  TOKEN '%ts' %ts%n", p->token,
				g_sh_token_str[(uintptr_t)p->token_data]);
	}
	ft_printf(p->eof ? "END CMD EOF%n" : "END CMD%n");
	return (true);
}

static bool		sh_parse_sub(t_parse_data *p)
{
	if (!sh_parse_cmd(p))
		return (false);
	if (p->eof)
		ft_printf("UNCLOSED SUB%n");
	return (true);
}

static bool		sh_parse_string(t_parse_data *p)
{
	ft_printf("  BEGIN STRING%n");
	while (parse_token(p))
		ft_printf("    STRING += '%ts' %ts%n", p->token, g_sh_token_str[(uintptr_t)p->token_data]);
	ft_printf(p->eof ? "  END STRING UNCLOSED%n" : "END STRING%n");
	return (true);
}

static bool				run_shell(t_sub str)
{
	static t_parser const	*sh_parser = NULL;
	t_in					sh_in;

	if (sh_parser == NULL
		&& (sh_parser = load_sh_parser()) == NULL)
		return (false);
	sh_in = IN(str.str, str.length, NULL);
	parse(&sh_in, sh_parser, NULL);
	return (true);
}

/*
** ========================================================================== **
** Init
*/

static bool		binding_runshell(t_main *main, t_editor *editor, t_key key)
{
	if (!run_shell(DSTR_SUB(editor->text)))
		ft_printf("RUN SHELL FAILED%n");
	return (true);
	(void)main;
	(void)key;
}

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
		editor_bind(main->editor, KEY('m', MOD_CTRL), CALLBACK(binding_runshell, main), 1);

		if ((main->syntax_color
				= load_syntax_color(SUBC(DEFAULT_SYNTAX_COLOR))) == NULL)
			ft_dprintf(2, WARNING_MSG("Cannot load syntax coloration: %s%n"),
				DEFAULT_SYNTAX_COLOR);
		main->flags |= FLAG_INTERACTIVE;
	}
	return (true);
}

/*
** ========================================================================== **
** Loop
*/

/*
lol;drxd
truclol&&
"sdfsadf"
\"looooool
$SDFOIWEF:&&
|||$((4+(8 - 1 * 2 / 7)))
lollolol
*/

static void		interactive_loop(t_main *main)
{
	t_key			key;
	int				key_used;
	t_vec2u			cursor;

	key_used = -1;
	ft_trestore(main->term, true);
	while (!(main->flags & FLAG_EXIT))
	{
		cursor = editor_rowcol(main->editor, main->editor->cursor);
		refresh_syntax(main->editor, main->syntax_color);
		ft_fprintf(&main->term->out, "[[ ");
		if (key_used >= 0)
		{
			ft_fprintf(&main->term->out, key_used ? C_GREEN : C_RED);
			put_key(&main->term->out, key);
			ft_putsub(&main->term->out, SUBC(C_RESET " "));
		}

		ft_fprintf(&main->term->out, "lines: %u; chars: %u; cursor: %u,%u (%u); spans: %u; line_len: %u ]]%n",
			main->editor->line_stops.length, main->editor->text.length,
			cursor.x, cursor.y, main->editor->cursor, main->editor->spans.spans.length,
			EDITOR_LINE(main->editor, cursor.y));

		ft_flush(&main->term->out);
		cursor.x += main->term->cursor_x;
		cursor.y += main->term->cursor_y;
		editor_out(main->editor, &main->term->out);
		ft_flush(&main->term->out);
		ft_tcursor(main->term, cursor.x, cursor.y);
		key = ft_getkey(0);
		ft_tclear(main->term);
		key_used = editor_key(main->editor, key);
		if (key.c == 'd' && key.mods == MOD_CTRL) // TMP
			main->flags |= FLAG_EXIT; // TMP

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
		return (1);
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
}
