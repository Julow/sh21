/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/10 00:47:17 by juloo             #+#    #+#             */
/*   Updated: 2016/09/11 15:21:52 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_colors.h"
#include "ft/ft_printf.h"
#include "ft/get_next_line.h"
#include "ft/getkey.h"
#include "ft/spanlist.h"
#include "ft/term.h"
#include "ft/tokenizer.h"
#include "sh/ast.h"
#include "sh/default_builtins.h"
#include "sh/exec.h"
#include "sh/parser.h"

#include "editor.h"
#include "editor_bindings.h"

// #include "syntax_color.h"

#include <termios.h>
#include <unistd.h>

#include <sys/select.h>

/*
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
	// t_syntax_color const	*syntax_color;
	t_sh_context		sh_context;
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

// #define SCOPE(S,C)	{SUBC(S), &STYLE C}

// static t_vector const	g_color_scheme = VECTOR(t_color_scheme,
// 	SCOPE("string.simple", (S_YELLOW, 0, 0)),
// 	SCOPE("string", (S_LIGHT(S_YELLOW), 0, 0)),
// 	SCOPE("escaped", (S_LIGHT(S_RED), 0, 0)),
// 	SCOPE("number", (S_YELLOW, 0, 0)),
// 	SCOPE("op", (S_LIGHT(S_WHITE), 0, 0)),
// 	SCOPE("subst", (S_CYAN, 0, 0)),
// 	SCOPE("comment", (0, S_BLUE, 0)),
// 	SCOPE("identifier.key", (S_LIGHT(S_CYAN), 0, 0)),
// 	SCOPE("identifier", (S_GREEN, 0, 0)),
// 	SCOPE("keyword", (S_MAGENTA, 0, 0)),
// 	SCOPE("error", (0, S_RED, 0)),
// );

// static void		sh_spanlist_fill(t_spanlist *spanlist,
// 					t_vec2u range, t_style const *style)
// {
// 	t_style			*tmp;

// 	tmp = ft_spanlist_push(spanlist, range.y - range.x, 1);
// 	*tmp = (style == NULL) ? STYLE(0, 0, 0) : *style;
// }

// static void		refresh_syntax(t_editor *editor, t_syntax_color const *s)
// {
// 	t_in			parse_in;

// 	parse_in = IN(editor->text.str, editor->text.length, NULL);
// 	ft_spanlist_clear(&editor->spans, 1);
// 	exec_syntax_color(&parse_in, s, &g_color_scheme,
// 		CALLBACK(sh_spanlist_fill, &editor->spans));
// }

/*
** ========================================================================== **
** Shell print
*/

#define PRINT_CMD(INDENT, FMT, ...)	ft_printf("%.*c" FMT, (INDENT) * 4 + 1, ' ', ##__VA_ARGS__)

static void		print_sh_compound(t_sh_compound const *cmd, uint32_t indent);
static void		print_sh_text(t_sh_text const *text, uint32_t indent);

static void		print_sh_subst_param_str(t_sh_subst_param const *param, uint32_t indent)
{
	print_sh_text(&param->str, indent);
}

static void		print_sh_subst_param_end(t_sh_subst_param const *param, uint32_t indent)
{
	(void)param;
	(void)indent;
}

static void		print_sh_subst_param_repl(t_sh_subst_param const *param, uint32_t indent)
{
	print_sh_text(&param->repl[0], indent);
	ft_printf("\033[36m/\033[0m");
	print_sh_text(&param->repl[1], indent);
}

static struct {
	char const		*op;
	void			(*f)(t_sh_subst_param const *param, uint32_t indent);
} const			g_print_sh_subst_param[] = {
	[SH_SUBST_PARAM_DEF_NULL] = {":-", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_DEF_UNSET] = {"-", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_ASSIGN_NULL] = {":=", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_ASSIGN_UNSET] = {"=", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REPL_NULL] = {":+", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REPL_UNSET] = {"+", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_ERR_NULL] = {":?", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_ERR_UNSET] = {"?", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_UPPER_FIRST] = {"^", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_UPPER] = {"^^", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_LOWER_FIRST] = {",", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_LOWER] = {",,", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_INVCASE_FIRST] = {"~", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_INVCASE] = {"~~", &print_sh_subst_param_end},
	[SH_SUBST_PARAM_REM_BEGIN] = {"#", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REM_BEGIN_LONG] = {"##", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REM_END] = {"%", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REM_END_LONG] = {"%%", &print_sh_subst_param_str},
	[SH_SUBST_PARAM_REPL_FIRST] = {"/", &print_sh_subst_param_repl},
	[SH_SUBST_PARAM_REPL_LAST] = {"/%", &print_sh_subst_param_repl},
	[SH_SUBST_PARAM_REPL_ALL] = {"//", &print_sh_subst_param_repl},
};

static uint32_t	print_sh_param(t_sh_text const *text, uint32_t token_start,
					t_sh_param const *param)
{
	if (param->type == SH_PARAM_STR)
	{
		ft_printf("\033[36m${\033[0m%ts\033[36m}\033[0m",
			SUB(text->text.str + token_start, param->str_length));
		return (param->str_length);
	}
	else if (param->type == SH_PARAM_LENGTH)
	{
		ft_printf("\033[36m${#\033[0m%ts\033[36m}\033[0m",
			SUB(text->text.str + token_start, param->str_length));
		return (param->str_length);
	}
	else if (param->type == SH_PARAM_POS)
		ft_printf("\033[36m$%d\033[0m", param->pos);
	else if (param->type == SH_PARAM_SPECIAL)
		ft_printf("\033[36m$%c\033[0m", ((char[]){
			[SH_SPECIAL_PARAM_ARGV] = '*',
			[SH_SPECIAL_PARAM_ARGV2] = '@',
			[SH_SPECIAL_PARAM_ARGC] = '#',
			[SH_SPECIAL_PARAM_STATUS] = '?',
			[SH_SPECIAL_PARAM_OPT] = '-',
			[SH_SPECIAL_PARAM_PID] = '$',
		})[param->special]);
	else
		ASSERT(!"Invalid sh_param type");
	return (0);
}

static void		print_sh_text(t_sh_text const *text, uint32_t indent)
{
	uint32_t			i;
	uint32_t			token_start;
	t_sh_token const	*token;

	i = 0;
	token_start = 0;
	while (i < text->tokens.length)
	{
		token = VECTOR_GET(text->tokens, i++);
		if (token->type & SH_F_T_QUOTED)
			ft_printf("\033[93m\"");
		switch (token->type & ~SH_F_T_QUOTED)
		{
		case SH_T_STRING:
			ft_printf("%ts",
				SUB(text->text.str + token_start, token->token_len));
			token_start += token->token_len;
			break ;
		case SH_T_SPACE:
			ASSERT(!(token->type & SH_F_T_QUOTED), "quoted space");
			ft_printf(" ");
			break ;
		case SH_T_SUBSHELL:
			ft_printf("\033[36m$(\033[0m%n");
			print_sh_compound(token->cmd, indent + 1);
			PRINT_CMD(indent, "\033[36m)\033[0m");
			break ;
		case SH_T_PARAM:
			token_start += print_sh_param(text, token_start, &token->param);
			break ;
		case SH_T_SUBST_PARAM:
			ft_printf("\033[36m${\033[0m");
			token_start += print_sh_param(text, token_start, &token->subst_param->param);
			ft_printf("\033[36m%s\033[0m",
				g_print_sh_subst_param[token->subst_param->type].op);
			g_print_sh_subst_param[token->subst_param->type].f(token->subst_param, indent + 1);
			ft_printf("\033[36m}\033[0m%n");
			break ;
		default:
			ft_printf("%n");
			PRINT_CMD(indent + 1, "<INVALID TOKEN TYPE> %u%n", token->type);
			PRINT_CMD(indent, "");
			break ;
		}
		if (token->type & SH_F_T_QUOTED)
			ft_printf("\033[93m\"\033[0m");
	}
}

static void		print_sh_do_clause(t_sh_compound const *cmd, uint32_t indent)
{
	PRINT_CMD(indent, "\033[33mdo\033[0m%n");
	print_sh_compound(cmd, indent + 1);
	PRINT_CMD(indent, "\033[33mdone\033[0m");
}

static void		print_sh_if_clause(t_sh_if const *cmd, uint32_t indent)
{
	print_sh_compound(&cmd->cond, indent + 1);
	PRINT_CMD(indent, "\033[33mthen\033[0m%n");
	print_sh_compound(&cmd->body, indent + 1);
	if (cmd->else_clause == NULL)
		PRINT_CMD(indent, "\033[33mfi\033[0m");
	else if (cmd->else_clause->type == SH_ELSE_ELIF)
	{
		PRINT_CMD(indent, "\033[33melif\033[0m%n");
		print_sh_if_clause(&cmd->else_clause->elif_clause, indent);
	}
	else if (cmd->else_clause->type == SH_ELSE_ELSE)
	{
		PRINT_CMD(indent, "\033[33melse\033[0m%n");
		print_sh_compound(&cmd->else_clause->else_clause, indent + 1);
		PRINT_CMD(indent, "\033[33mfi\033[0m");
	}
}

static void		print_sh_redir_lst(t_sh_redir_lst const *redirs, uint32_t indent)
{
	t_sh_redir const	*redir;

	redir = VECTOR_IT(redirs->redirs);
	while (VECTOR_NEXT(redirs->redirs, redir))
	{
		ft_printf(" ");
		if (redir->left_fd >= 0)
			ft_printf("\033[33m%d\033[0m", redir->left_fd);
		ft_printf("\033[36m%s\033[0m", ((char const*[]){
			[SH_REDIR_OUTPUT] = ">",
			[SH_REDIR_OUTPUT_CLOBBER] = ">|",
			[SH_REDIR_APPEND] = ">>",
			[SH_REDIR_INPUT] = "<",
			[SH_REDIR_INPUT_FD] = "<&",
			[SH_REDIR_OUTPUT_FD] = ">&",
			[SH_REDIR_OPEN] = "<>",
		})[redir->type]);
		print_sh_text(&redir->right_text, indent);
	}
}

static void		print_sh_cmd(t_sh_cmd const *cmd, uint32_t indent)
{
	switch (cmd->type)
	{
	case SH_CMD_SIMPLE:
		print_sh_text(&cmd->simple.text, indent);
		break ;
	case SH_CMD_SUBSHELL:
		ft_printf("\033[33m(\033[0m%n");
		print_sh_compound(cmd->subshell, indent + 1);
		PRINT_CMD(indent, "\033[33m)\033[0m");
		break ;
	case SH_CMD_IF_CLAUSE:
		ft_printf("\033[33mif\033[0m%n");
		print_sh_if_clause(cmd->if_clause, indent);
		break ;
	case SH_CMD_FOR_CLAUSE:
		ft_printf("\033[33mfor\033[0m %ts \033[33min\033[0m ",
			cmd->for_clause->var);
		print_sh_text(&cmd->for_clause->data, indent);
		ft_printf("%n");
		print_sh_do_clause(&cmd->for_clause->body, indent);
		break ;
	case SH_CMD_WHILE_CLAUSE:
	case SH_CMD_UNTIL_CLAUSE:
		ft_printf("\033[33m%s\033[0m%n",
			(cmd->type == SH_CMD_WHILE_CLAUSE) ? "while" : "until");
		print_sh_compound(&cmd->while_clause->cond, indent + 1);
		print_sh_do_clause(&cmd->while_clause->body, indent);
		break ;
	case SH_CMD_TIME_CLAUSE:
		ft_printf("\033[33mtime\033[0m ");
		print_sh_cmd(cmd->rec, indent);
		break ;
	case SH_CMD_NOT_CLAUSE:
		ft_printf("\033[33m!\033[0m ");
		print_sh_cmd(cmd->rec, indent);
		break ;
	case SH_CMD_BRACKET_CLAUSE:
		ft_printf("\033[33m{\033[0m%n");
		PRINT_CMD(indent, "");
		print_sh_compound(cmd->bracket_clause, indent + 1);
		PRINT_CMD(indent, "\033[33m}\033[0m");
		break ;
	case SH_CMD_FUNCTION_DEF:
		ft_printf("\033[33mfunction\033[0m \033[36m%ts\033[0m ",
			cmd->function_def->name);
		print_sh_cmd(&cmd->function_def->body, indent + 1);
		break ;
	}
	print_sh_redir_lst(&cmd->redirs, indent);
}

static void		print_sh_pipeline(t_sh_pipeline const *cmd, uint32_t indent)
{
	while (cmd != NULL)
	{
		print_sh_cmd(&cmd->cmd, indent);
		if (cmd->next == NULL)
			break ;
		ft_printf(" \033[97m|\033[0m ");
		cmd = cmd->next;
	}
}

static void		print_sh_list(t_sh_list const *cmd, uint32_t indent)
{
	while (true)
	{
		print_sh_pipeline(&cmd->pipeline, indent);
		if (cmd->next == NULL)
			break ;
		ft_printf(" \033[97m%s\033[0m ", (cmd->next->type == SH_LIST_AND) ? "&&" : "||");
		cmd = &cmd->next->next;
	}
}

static void 	print_sh_compound(t_sh_compound const *cmd, uint32_t indent)
{
	while (cmd != NULL)
	{
		PRINT_CMD(indent, "");
		print_sh_list(&cmd->list, indent);
		ft_printf(" \033[97m%s\033[0m%n", (cmd->flags & SH_COMPOUND_ASYNC) ? "&" : ";");
		cmd = cmd->next;
	}
}

/*
** ========================================================================== **
** Init
*/

#define MOD_CTRL_X		(1 << 8)

#include <stdlib.h>

static bool		binding_runshell(t_main *main, t_editor *editor, t_key key)
{
	t_sub const		text = DSTR_SUB(editor->text);
	t_in			in;
	t_sh_compound	cmd;
	t_sh_parse_err	*err;

	in = IN(text.str, text.length, NULL);
	while (IN_REFRESH(&in))
	{
		// TMP
		if ((err = sh_parse(&in, &cmd)) != NULL)
		{
			if (err->type == SH_E_UNTERMINATED)
				ft_printf("ERROR: unterminated '%s'%n", ((char const*[]){
					[SH_E_UNTERMINATED_AND] = "&&",
					[SH_E_UNTERMINATED_OR] = "||",
					[SH_E_UNTERMINATED_PIPE] = "|",
					[SH_E_UNTERMINATED_SUBSHELL] = "(",
					[SH_E_UNTERMINATED_SUBST_SUBSHELL] = "$(",
					[SH_E_UNTERMINATED_SUBST_BACKQUOTE] = "`",
					[SH_E_UNTERMINATED_SUBST_BACKQUOTE_REV] = "\\`",
					[SH_E_UNTERMINATED_BRACKET] = "{",
					[SH_E_UNTERMINATED_IF] = "if",
					[SH_E_UNTERMINATED_THEN] = "then",
					[SH_E_UNTERMINATED_ELSE] = "else",
					[SH_E_UNTERMINATED_WHILE] = "while",
					[SH_E_UNTERMINATED_FOR] = "for",
					[SH_E_UNTERMINATED_IN] = "in",
					[SH_E_UNTERMINATED_DO] = "do",
					[SH_E_UNTERMINATED_LINE] = "\\",
					[SH_E_UNTERMINATED_STRING] = "\"",
					[SH_E_UNTERMINATED_STRING_SINGLE] = "'",
					[SH_E_UNTERMINATED_STRING_ANSI] = "$'",
				})[err->unterminated]);
			else if (err->type == SH_E_UNEXPECTED)
				ft_printf("ERROR: unexpected token '%ts'%n", err->token);
			else if (err->type == SH_E_UNEXPECTED)
				ft_printf("ERROR: invalid token '%ts'%n", err->token);
			else if (err->type == SH_E_EOF)
				ft_printf("ERROR: unexpected eof%n");
			else
				ft_printf("ERROR: error wtf%n");

			free(err);
			break ;
		}

		print_sh_compound(&cmd, 0);
		sh_exec_compound(&main->sh_context, &cmd, false);
		sh_destroy_compound(&cmd);
	}
	return (true);
	(void)key;
}

static bool		binding_runshell_one(t_main *main, t_editor *editor, t_key key)
{
	if (editor->line_stops.length > 1)
		return (false);
	return (binding_runshell(main, editor, key));
}

static bool		init_main(t_main *main, char const *const *argv)
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

		// if ((main->syntax_color
		// 		= load_syntax_color(SUBC(DEFAULT_SYNTAX_COLOR))) == NULL)
		// 	ft_dprintf(2, WARNING_MSG("Cannot load syntax coloration: %s%n"),
		// 		DEFAULT_SYNTAX_COLOR);
		main->flags |= FLAG_INTERACTIVE;
	}
	sh_context_init(&main->sh_context, argv);
	sh_init_default_builtins(&main->sh_context);
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
		// refresh_syntax(main->editor, main->syntax_color);
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

int				main(int argc, char const *const *argv)
{
	t_main			main;

	if (!init_main(&main, argv))
		return (1);
	if (main.flags & FLAG_INTERACTIVE)
		interactive_loop(&main);
	else
		loop(&main);
	return (0);
	(void)argc;
}
