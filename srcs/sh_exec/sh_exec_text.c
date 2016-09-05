/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 18:50:45 by juloo             #+#    #+#             */
/*   Updated: 2016/09/05 18:06:11 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <unistd.h>

static uint32_t	exec_string(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst)
{
	ft_dstradd(dst, SUB(text.str, t->token_len));
	return (t->token_len);
	(void)c;
}

static uint32_t	exec_space(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst)
{
	DSTR_APPEND(dst, ' '); // TODO: replace SPACE tokens by STRING token ?
	(void)c;
	(void)t;
	(void)text;
	return (0);
}

static uint32_t	exec_subshell(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst)
{
	int				pipe_fds[2];
	int				len;
	pid_t			pid;
	char			buff[256];

	if (pipe(pipe_fds) < 0)
		return (ASSERT(!"pipe fail"), 0);
	if ((pid = fork()) < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), ASSERT(!"fork fail"), 0);
	if (pid == 0)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], 1);
		sh_exec_compound(c, t->cmd, true);
		HARD_ASSERT(false);
	}
	close(pipe_fds[1]);
	while ((len = read(pipe_fds[0], buff, sizeof(buff))) > 0)
		ft_dstradd(dst, SUB(buff, len));
	while (dst->length > 0 && dst->str[dst->length - 1] == '\n')
		dst->length--;
	close(pipe_fds[0]);
	return (0);
	(void)text;
}

static uint32_t	exec_param(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst)
{
	t_sub			val;

	if (t->param.type == SH_PARAM_STR)
	{
		val = sh_var_get(c, SUB(text.str, t->param.str_length));
		ft_dstradd(dst, val);
		return (t->param.str_length);
	}
	ASSERT(!"TODO: exec_param: SH_PARAM_LENGTH, SH_PARAM_POS, SH_PARAM_SPECIAL");
	return (0);
}

static uint32_t	exec_subst_param(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst)
{
	ASSERT(!"TODO: exec_text subst_param");
	return (0);
	(void)c; (void)t; (void)text; (void)dst;
}

static uint32_t	(*const g_sh_exec_text[])(t_sh_context *c, t_sh_token const *t,
					t_sub text, t_dstr *dst) = {
	[SH_T_STRING] = &exec_string,
	[SH_T_SPACE] = &exec_space,
	[SH_T_PARAM] = &exec_param,
	[SH_T_SUBSHELL] = &exec_subshell,
	[SH_T_SUBST_PARAM] = &exec_subst_param,
};

static uint32_t	split_words(t_dstr *str, uint32_t offset)
{
	uint32_t		word_count;
	uint32_t		i;

	word_count = 0;
	while (offset < str->length && !IS(str->str[offset], IS_SPACE))
		offset++;
	i = offset;
	while (offset < str->length)
	{
		while (++offset < str->length && IS(str->str[offset], IS_SPACE))
			;
		word_count++;
		str->str[i++] = '\0';
		while (offset < str->length && !IS(str->str[offset], IS_SPACE))
			str->str[i++] = str->str[offset++];
	}
	return (word_count);
}

uint32_t		sh_exec_text(t_sh_context *c,
					t_sh_text const *text, t_dstr *dst)
{
	uint32_t			param_count;
	uint32_t			text_i;
	uint32_t			i;
	uint32_t			tmp;
	t_sh_token const	*t;

	text_i = 0;
	i = 0;
	tmp = 0;
	param_count = 0;
	while (i < text->tokens.length)
	{
		t = VECTOR_GET(text->tokens, i);
		tmp = dst->length;
		text_i += g_sh_exec_text[t->type & ~SH_F_T_QUOTED](c, t,
					SUB_FOR(DSTR_SUB(text->text), text_i), dst);
		if (!(t->type & SH_F_T_QUOTED))
			param_count += split_words(dst, tmp);
		i++;
	}
	DSTR_APPEND(dst, '\0');
	return (param_count + 1);
}
