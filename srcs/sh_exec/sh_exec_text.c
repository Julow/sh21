/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 18:50:45 by juloo             #+#    #+#             */
/*   Updated: 2016/09/06 18:13:10 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <unistd.h>

static void		load_ifs(t_sh_context const *c, t_bits *dst)
{
	t_sub			ifs_val;
	uint32_t		i;

	ifs_val = sh_var_get(c, SUBC("IFS"));
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
	load_ifs(c, ifs);
	i = 0;
	tmp = 0;

	while (i < append.length)
	{
		if (BITARRAY_GET(ifs, append.str[i]))
		{
			ft_str_list_append(dst, SUB(append.str + tmp, i - tmp));
			ft_str_list_break(dst);
			if (IS(append.str[i], IS_BLANK))
				while (BITARRAY_GET(ifs, append.str[i + 1]))
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
		close(pipe_fds[0]);
		dup2(pipe_fds[1], 1);
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

static void		sh_exec_text_t_param(t_sh_context *c, t_sh_exec_text *e,
					t_sh_token const *t, t_str_list *dst)
{
	t_sub			val;

	if (t->param.type == SH_PARAM_STR)
	{
		val = sh_var_get(c, SUB(e->text->text.str + e->str_i, t->param.str_length));
		e->str_i += t->param.str_length;
		sh_exec_text_append_ifs(c, t, val, dst);
	}
	else
		ASSERT(!"TODO: exec_param: SH_PARAM_LENGTH, SH_PARAM_POS, SH_PARAM_SPECIAL");
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
