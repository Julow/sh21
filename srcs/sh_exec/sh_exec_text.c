/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 18:50:45 by juloo             #+#    #+#             */
/*   Updated: 2016/09/07 16:38:17 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/ft_printf.h" // TODO: split {s,a,as,f}printf and {,d}printf
#include "p_sh_exec.h"

#include <unistd.h>

static void		load_ifs(t_sh_context const *c, t_bits *dst)
{
	t_sub			ifs_val;
	uint32_t		i;

	ifs_val = sh_c_var_get(c, SUBC("IFS"));
	if (ifs_val.length == 0)
		ifs_val = SUBC(" \t\n");
	ft_bitclear(dst, 0, 256);
	i = 0;
	while (i < ifs_val.length)
	{
		BITARRAY_SET(dst, ifs_val.str[i]);
		i++;
	}
}

static void		sh_exec_text_append_ifs(t_sh_context const *c,
					t_sh_token const *t, t_sub append, t_str_list *dst)
{
	t_bits			ifs[BITARRAY(256)];
	uint32_t		i;
	uint32_t		tmp;

	if (t->type & SH_F_T_QUOTED)
	{
		ft_str_list_append(dst, append);
		return ;
	}
	load_ifs(c, ifs); // TODO: cache result
	i = 0;
	tmp = 0;

	while (i < append.length)
	{
		if (BITARRAY_GET(ifs, append.str[i]))
		{
			ft_str_list_append(dst, SUB(append.str + tmp, i - tmp));
			ft_str_list_break(dst);
			if (IS(append.str[i], IS_BLANK))
				while ((i + 1) < append.length
					&& BITARRAY_GET(ifs, append.str[i + 1]))
					i++;
			tmp = ++i;
			continue ;
		}
		i++;
	}
	if (i > tmp)
		ft_str_list_append(dst, SUB(append.str + tmp, i - tmp));
}

static void		sh_exec_text_t_string(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	ft_str_list_append(dst, SUB(e->text->text.str + e->str_i, t->token_len));
	e->str_i += t->token_len;
	(void)c;
}

static void		sh_exec_text_t_space(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	ft_str_list_break(dst);
	(void)c;
	(void)e;
	(void)t;
}

static void		sh_exec_text_t_subshell(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	int				pipe_fds[2];
	int				len;
	pid_t			pid;
	t_dstr			buff;

	if (pipe(pipe_fds) < 0)
		return (ASSERT(!"pipe fail"), VOID);
	if ((pid = fork()) < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), ASSERT(!"fork fail"), VOID);
	if (pid == 0)
	{
		dup2(pipe_fds[1], 1);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		sh_exec_compound(c, t->cmd, true);
		HARD_ASSERT(false);
	}
	close(pipe_fds[1]);
	buff = DSTR0();
	while (true)
	{
		DSTR_EXTEND(&buff, 512);
		if ((len = read(pipe_fds[0], buff.str + buff.length, buff.capacity - buff.length)) <= 0)
			break ;
		buff.length += len;
	}
	close(pipe_fds[0]);
	while (buff.length > 0 && buff.str[buff.length - 1] == '\n')
		buff.length--;
	sh_exec_text_append_ifs(c, t, DSTR_SUB(buff), dst);
	ft_dstrclear(&buff);
	(void)e;
}

static void		sh_exec_text_t_param_special_argv(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	t_sub const		ifs_val = sh_c_var_get(c, SUBC("IFS"));
	char const		sep = (ifs_val.length >= 1) ? ifs_val.str[0] : ' ';
	uint32_t		offset;

	if (c->pos_params.count == 0)
	{
		sh_exec_text_append_ifs(c, t, SUB0(), dst);
		return ;
	}
	offset = 0;
	while (true)
	{
		sh_exec_text_append_ifs(c, t, STR_LIST_SUB(c->pos_params, offset), dst);
		offset += STR_LIST_NEXT(c->pos_params, offset);
		if (offset >= STR_LIST_END(c->pos_params))
			break ;
		sh_exec_text_append_ifs(c, t, SUB(&sep, 1), dst);
	}
}

static void		sh_exec_text_t_param_special_argv2(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	uint32_t		offset;

	if (!(t->type & SH_F_T_QUOTED) || c->pos_params.count == 0)
	{
		sh_exec_text_t_param_special_argv(c, t, dst);
		return ;
	}
	offset = 0;
	while (true)
	{
		ft_str_list_append(dst, STR_LIST_SUB(c->pos_params, offset));
		offset += STR_LIST_NEXT(c->pos_params, offset);
		if (offset >= STR_LIST_END(c->pos_params))
			break ;
		ft_str_list_break(dst);
	}
}

static void		sh_exec_text_t_param_special_argc(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	char			buff[16];
	uint32_t		len;

	len = ft_sprintf(buff, "%u", c->pos_params.count);
	sh_exec_text_append_ifs(c, t, SUB(buff, len), dst);
}

static void		sh_exec_text_t_param_special_status(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	char			buff[16];
	uint32_t		len;

	len = ft_sprintf(buff, "%u", c->last_status);
	sh_exec_text_append_ifs(c, t, SUB(buff, len), dst);
}

static void		sh_exec_text_t_param_special_opt(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	ASSERT(!"TODO: exec param special opt");
	sh_exec_text_append_ifs(c, t, SUB0(), dst);
}

static void		sh_exec_text_t_param_special_pid(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst)
{
	pid_t const		pid = getpid();
	char			buff[16];
	uint32_t		len;

	len = ft_sprintf(buff, "%d", pid);
	sh_exec_text_append_ifs(c, t, SUB(buff, len), dst);
}

static void		(*g_sh_exec_text_t_param_special[])(t_sh_context *c,
					t_sh_token const *t, t_str_list *dst) = {
	[SH_SPECIAL_PARAM_ARGV] = &sh_exec_text_t_param_special_argv,
	[SH_SPECIAL_PARAM_ARGV2] = &sh_exec_text_t_param_special_argv2,
	[SH_SPECIAL_PARAM_ARGC] = &sh_exec_text_t_param_special_argc,
	[SH_SPECIAL_PARAM_STATUS] = &sh_exec_text_t_param_special_status,
	[SH_SPECIAL_PARAM_OPT] = &sh_exec_text_t_param_special_opt,
	[SH_SPECIAL_PARAM_PID] = &sh_exec_text_t_param_special_pid,
};

static t_sub	ft_str_list_nth(t_str_list const *lst, uint32_t nth) // TODO: move
{
	uint32_t		offset;

	offset = 0;
	while (nth-- > 0)
		offset += STR_LIST_NEXT(*lst, offset);
	return (STR_LIST_SUB(*lst, offset));
}

static void		sh_exec_text_t_param(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	t_sub			val;
	char			tmp[16];

	if (t->param.type == SH_PARAM_STR)
	{
		val = sh_c_var_get(c, SUB(e->text->text.str + e->str_i, t->param.str_length));
		e->str_i += t->param.str_length;
		sh_exec_text_append_ifs(c, t, val, dst);
	}
	else if (t->param.type == SH_PARAM_LENGTH)
	{
		val = sh_c_var_get(c, SUB(e->text->text.str + e->str_i, t->param.str_length));
		e->str_i += t->param.str_length;
		val.length = ft_sprintf(tmp, "%u", val.length);
		val.str = tmp;
		ft_str_list_append(dst, val);
	}
	else if (t->param.type == SH_PARAM_POS)
	{
		val = (t->param.pos < c->pos_params.count) ?
			ft_str_list_nth(&c->pos_params, t->param.pos) : SUB0();
		sh_exec_text_append_ifs(c, t, val, dst);
	}
	else
		g_sh_exec_text_t_param_special[t->param.special](c, t, dst);
}

static void		sh_exec_text_t_subst_param(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	ASSERT(!"TODO: exec_subst_param");
	(void)c;
	(void)e;
	(void)t;
	(void)dst;
}

static void		(*g_sh_exec_text[])(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst) = {
	[SH_T_STRING] = &sh_exec_text_t_string,
	[SH_T_SPACE] = &sh_exec_text_t_space,
	[SH_T_SUBSHELL] = &sh_exec_text_t_subshell,
	[SH_T_PARAM] = &sh_exec_text_t_param,
	[SH_T_SUBST_PARAM] = &sh_exec_text_t_subst_param,
};

void			sh_exec_text(t_sh_context *c, t_sh_exec_text *e,
					uint32_t count, t_str_list *dst)
{
	t_sh_token const	*t;

	count = (count == 0) ? e->text->tokens.length : count + e->token_i;
	while (e->token_i < count)
	{
		t = VECTOR_GET(e->text->tokens, e->token_i);
		g_sh_exec_text[t->type & ~SH_F_T_QUOTED](c, e, t, dst);
		e->token_i++;
	}
}
