/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/02/12 13:00:10 by jaguillo         ###   ########.fr       */
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
#include "sh/tokens.h"

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

#define PRINT_CMD(INDENT, FMT, ...)	ft_printf("%.*c" FMT, INDENT, '\t', ##__VA_ARGS__)

static void		print_cmd(t_sh_cmd const *cmd, uint32_t indent);

static char const *const	g_print_subst_type[] = {
	[SH_SUBST_PARAM] = "PARAM",
	[SH_SUBST_STRLEN] = "STRLEN",
	[SH_SUBST_EXPR] = "EXPR",
	[SH_SUBST_MATH] = "MATH",
	[SH_SUBST_CMD] = "CMD",
};

static void		print_subst(t_sh_subst const *subst, uint32_t indent)
{
	PRINT_CMD(indent, "[%u -> %u] %s%n", subst->range.x, subst->range.y,
		(subst->type >= ARRAY_LEN(g_print_subst_type))
		? "<INVALID SUBST TYPE>" : g_print_subst_type[subst->type]);
	switch (subst->type)
	{
	case SH_SUBST_CMD:
		print_cmd(subst->val.cmd, indent + 1);
		break ;
	default:
		break ;
	}
}

static void		print_cmd_simple(t_sh_cmd const *cmd, uint32_t indent)
{
	t_sh_simple_cmd const *const	scmd = &cmd->val.cmd;
	uint32_t						i;
	t_vec2u							range;

	PRINT_CMD(indent, "CMD DATA: %ts%n", DSTR_SUB(scmd->text));
	PRINT_CMD(indent, "ARG STOPS:");
	i = 0;
	range = VEC2U(0, 0);
	while (range.x < scmd->text.length)
	{
		if (i >= scmd->arg_stops.length)
			range.y = scmd->text.length;
		else
			range.y = *(uint32_t*)VECTOR_GET(scmd->arg_stops, i++);
		ft_printf(" %ts", SUB(scmd->text.str + range.x, range.y - range.x));
		range.x = range.y;
	}
	ft_printf("%n");
	PRINT_CMD(indent, "SUBSTS: [%n");
	i = 0;
	while (i < scmd->substs.length)
		print_subst(VECTOR_GET(scmd->substs, i++), indent + 1);
	PRINT_CMD(indent, "]%n");
}

static void		(*g_print_cmd[])(t_sh_cmd const *cmd, uint32_t indent) = {
	[SH_CMD_SIMPLE] = &print_cmd_simple,
};

static char const *const	g_print_cmd_next[] = {
	[SH_NEXT_AND] = "&&",
	[SH_NEXT_OR] = "||",
	[SH_NEXT_PIPE] = "|",
	[SH_NEXT_NEW] = ";",
};

static char const *const	g_print_cmd_redir[] = {
	[SH_REDIR_INPUT] = "<",
	[SH_REDIR_OUTPUT] = ">",
	[SH_REDIR_APPEND] = ">>",
	[SH_REDIR_HEREDOC] = "<<",
};

static void		print_cmd_redir(t_list const *lst, uint32_t indent)
{
	t_sh_redir const	*redir;

	PRINT_CMD(indent, "REDIRS: [%n");
	redir = LIST_IT(lst);
	while ((redir = LIST_NEXT(redir)))
	{
		PRINT_CMD(indent + 1, "%u%s", redir->left_fd, g_print_cmd_redir[redir->type]);
		if (redir->right_type == SH_REDIR_RIGHT_FD)
			ft_printf("%u%n", redir->right.fd);
		else
			ft_printf("%ts%n", SUB(ENDOF(redir), redir->right.file_len));
	}
	PRINT_CMD(indent, "]%n");
}

static void		print_cmd(t_sh_cmd const *cmd, uint32_t indent)
{
	while (cmd != NULL)
	{
		PRINT_CMD(indent, "{%n");
		if (cmd->async)
			PRINT_CMD(indent + 1, "ASYNC%n");
		print_cmd_redir(&cmd->redirs, indent + 1);
		if (cmd->type >= ARRAY_LEN(g_print_cmd))
			PRINT_CMD(indent + 1, "<INVALID CMD TYPE>%n");
		else
			g_print_cmd[cmd->type](cmd, indent + 1);
		PRINT_CMD(indent, "}%n");
		PRINT_CMD(indent, "%s%n", (cmd->next_type >= ARRAY_LEN(g_print_cmd_next))
			? "<INVALID NEXT TYPE>" : g_print_cmd_next[cmd->next_type]);
		cmd = cmd->next;
	}
}

static bool		run_shell(t_sub str)
{
	t_in			sh_in;
	t_parse_data	p;
	t_sh_cmd		*cmd;
	t_sh_cmd		*first;

	sh_in = IN(str.str, str.length, NULL);
	p = PARSE_DATA(NULL, &sh_in);
	cmd = NULL;
	first = NULL;
	ft_printf("PARSE '%ts' [[%n", str);
	while (!p.eof)
	{
		p.env = (first == NULL) ? &first : &cmd->next;
		parse_frame(&p, load_sh_parser());
		cmd = *(void**)p.env;
		ASSERT(cmd != NULL);
		if (cmd->next_type == SH_NEXT_NEW)
		{
			ft_printf("RUN%n");
			print_cmd(first, 1);
			// free
			first = NULL;
		}
	}
	if (first != NULL)
	{
		ASSERT(false, "Unexpected end of file");
		ft_printf(">> '%ts' %u%n", p.token, p.token_data);
	}
	ft_printf("]]%n");
	D_PARSE_DATA(p);
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
