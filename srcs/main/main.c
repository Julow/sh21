/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/07/09 15:16:45 by jaguillo         ###   ########.fr       */
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
#include "sh/cmd.h"
#include "sh/exec.h"
#include "sh/parser.h"

#include "editor.h"
#include "editor_bindings.h"
#include "syntax_color.h"

#include <termios.h>
#include <unistd.h>

#include <sys/select.h>

/*
** TODO: parser: multi inherit
** TODO: parser: .tail=true to change parser without recursion
** TODO: editor: multi cursor
** TODO: editor: undo/redo history
** TODO: tokenizer: reuse buffer
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

#define PRINT_CMD(INDENT, FMT, ...)	ft_printf("%.*c" FMT, (INDENT) * 4 + 1, ' ', ##__VA_ARGS__)

static void		print_cmd(t_sh_cmd const *cmd, uint32_t indent);

static char const *const	g_redir_types[] = {
	[SH_REDIR_OUTPUT] = "OUTPUT",
	[SH_REDIR_OUTPUT_CLOBBER] = "OUTPUT_CLOBBER",
	[SH_REDIR_APPEND] = "APPEND",
	[SH_REDIR_INPUT] = "INPUT",
	[SH_REDIR_HEREDOC] = "HEREDOC",
	[SH_REDIR_DUP_INPUT] = "DUP_INPUT",
	[SH_REDIR_DUP_OUTPUT] = "DUP_OUTPUT",
	[SH_REDIR_OPEN] = "OPEN",
};

static char const *const	g_expr_types[] = {
	[SH_EXPR_USE_DEF] = "-",
	[SH_EXPR_SET_DEF] = "=",
	[SH_EXPR_ISSET] = "?",
	[SH_EXPR_USE_ALT] = "+",
	[SH_EXPR_SUFFIX] = "%",
	[SH_EXPR_PREFIX] = "#",
};

static void		print_sh_text(t_sh_text const *text, uint32_t indent)
{
	uint32_t			i;
	uint32_t			token_start;
	t_sh_token const	*token;

	PRINT_CMD(indent, "TEXT DATA: '%ts'%n", DSTR_SUB(text->text));
	PRINT_CMD(indent, "TOKENS: [%n");
	i = 0;
	token_start = 0;
	while (i < text->tokens.length)
	{
		token = VECTOR_GET(text->tokens, i++);
		char const *const	quoted = (token->type & SH_F_T_QUOTED) ? " (quoted)" : "";
		switch (token->type & ~SH_F_T_QUOTED)
		{
		case SH_T_STRING:
			PRINT_CMD(indent + 1, "SH_T_STRING%s '%ts'%n", quoted,
					SUB(text->text.str + token_start, token->val.token_len));
			token_start += token->val.token_len;
			break ;
		case SH_T_SPACE:
			PRINT_CMD(indent + 1, "SH_T_SPACE%s%n", quoted);
			break ;
		case SH_T_SUBSHELL:
			PRINT_CMD(indent + 1, "SH_T_SUBSHELL%s {%n", quoted);
			print_cmd(token->val.cmd, indent + 2);
			PRINT_CMD(indent + 1, "}%n");
			break ;
		case SH_T_REDIR:
			PRINT_CMD(indent + 1, "REDIR%s %s%n", quoted,
				g_redir_types[token->val.redir_type]);
			break ;
		case SH_T_PARAM:
			PRINT_CMD(indent + 1, "SH_T_PARAM%s ${%ts}%n", quoted,
				SUB(text->text.str + token_start, token->val.token_len));
			token_start += token->val.token_len;
			break ;
		case SH_T_EXPR:
			PRINT_CMD(indent + 1, "SH_T_EXPR%s ${%ts%s%s%n", quoted,
				SUB(ENDOF(token->val.expr), token->val.expr->param_len),
				(token->val.expr->type & SH_EXPR_F_ALT) ? ":" : "",
				g_expr_types[token->val.expr->type & ~SH_EXPR_F_ALT]);
			print_sh_text(&token->val.expr->text, indent + 2);
			PRINT_CMD(indent + 1, "}%n");
			break ;
		default:
			PRINT_CMD(indent + 1, "<INVALID TOKEN TYPE> %u%n", token->type);
			break ;
		}
	}
	PRINT_CMD(indent, "]%n");
}

static void		print_cmd(t_sh_cmd const *cmd, uint32_t indent)
{
	PRINT_CMD(indent, "{%n");
	print_sh_text(&cmd->text, indent + 2);
	PRINT_CMD(indent, "}%n");
	if (cmd->next_type == SH_NEXT_AND)
		PRINT_CMD(indent, "&&%n");
	else if (cmd->next_type == SH_NEXT_OR)
		PRINT_CMD(indent, "||%n");
	else if (cmd->next_type == SH_NEXT_PIPE)
		PRINT_CMD(indent, "|%n");
	else if (cmd->next_type == SH_NEXT_SEQ)
		PRINT_CMD(indent, ";%n");
	else if (cmd->next_type == SH_NEXT_ASYNC)
		PRINT_CMD(indent, "&%n");
	else
		ASSERT(false, "Invalid next type");
	if (cmd->next != NULL)
		print_cmd(cmd->next, indent);
}

/*
** ========================================================================== **
** Exec shell
*/

// TODO: fix PARSE_EOF() from ft/parser.h
#define PARSE_EOF2(P)	(!IN_REFRESH((P)->t.in))

// TODO: move to module sh_parser
t_sh_cmd		*sh_parse_compound(t_in *in, t_dstr *err)
{
	t_parse_data	p;
	t_sh_cmd		*cmd;
	t_sh_cmd		*first;

	p = PARSE_DATA(NULL, in);
	cmd = NULL;
	first = NULL;
	while (!PARSE_EOF(&p) && !PARSE_ERROR(&p))
	{

		if (!ft_parse(&p, load_sh_parser()))
		{
			if (first != NULL)
				sh_destroy_cmd(first);
			ft_asprintf(err, "%ts at token %ts", p.token, DSTR_SUB(p.t.buff));
			return (NULL);
		}
		cmd = (first == NULL) ? (first = p.env) : (cmd->next = p.env);
		if (cmd->next_type == SH_NEXT_SEQ)
			return (first);

		// TODO: return error using t_parse_data.env, change ft_parse_error to:
		// 	bool ft_parse_error(t_parse_data *p);
		// TODO: return error as error_code (enum),
		// 	use UNEXPECTED_EOF and UNCLOSED_* errors to ask for more lines

	}
	D_PARSE_DATA(p);
	return (first);
}

static bool		run_shell(t_sub str)
{
	t_in			in;
	t_sh_cmd		*cmd;
	t_dstr			err;

	ft_printf("%.20(=)%n");
	ft_printf("PARSE '%ts' [[%n", str);
	in = IN(str.str, str.length, NULL);
	err = DSTR0();
	while ((cmd = sh_parse_compound(&in, &err)) != NULL)
	{
		print_cmd(cmd, 0);
		sh_exec_cmd(NULL, cmd);
		sh_destroy_cmd(cmd);
		if (!ASSERT(cmd->text.text.length > 0))
			return (false);
	}
	if (err.length > 0)
	{
		ft_printf("Parse error: %ts%n", DSTR_SUB(err));
		ft_dstrclear(&err);
		return (false);
	}
	ft_printf("]]%n");
	return (true);
}

/*
** ========================================================================== **
** Init
*/

#define MOD_CTRL_X		(1 << 8)

static bool		binding_runshell(t_main *main, t_editor *editor, t_key key)
{
	if (!run_shell(DSTR_SUB(editor->text)))
		ft_printf("RUN SHELL FAILED%n");
	return (true);
	(void)main;
	(void)key;
}

static bool		binding_runshell_one(t_main *main, t_editor *editor, t_key key)
{
	if (editor->line_stops.length > 1)
		return (false);
	return (binding_runshell(main, editor, key));
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
		editor_bind(main->editor, KEY('x', MOD_CTRL), CALLBACK(editor_bind_extra_mod, V(MOD_CTRL_X)), 0);
		editor_bind(main->editor, KEY('m', MOD_CTRL), CALLBACK(binding_runshell_one, main), 1);
		editor_bind(main->editor, KEY('m', MOD_CTRL | MOD_CTRL_X), CALLBACK(binding_runshell, main), 0);
		editor_bind(main->editor, KEY('n', MOD_CTRL), CALLBACK(editor_bind_write, "\n"), 0);

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
