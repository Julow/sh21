/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_exec_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/23 18:28:27 by juloo             #+#    #+#             */
/*   Updated: 2016/09/06 17:41:54 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"

#include <fcntl.h>
#include <unistd.h>

struct {
	uint32_t		def_left;
	bool			right_int;
	bool			right_opened;
	int				open_flags;
} const			g_sh_exec_redir[] = {
	[SH_REDIR_OUTPUT] = {1, false, true, O_WRONLY | O_CREAT | O_TRUNC},
	[SH_REDIR_OUTPUT_CLOBBER] = {1, false, true, O_WRONLY | O_CREAT | O_TRUNC},
	[SH_REDIR_APPEND] = {1, false, true, O_WRONLY | O_CREAT | O_APPEND},
	[SH_REDIR_INPUT] = {0, false, true, O_RDONLY},
	[SH_REDIR_INPUT_FD] = {0, true, false, 0},
	[SH_REDIR_OUTPUT_FD] = {1, true, false, 0},
	[SH_REDIR_OPEN] = {1, false, true, O_RDWR | O_CREAT | O_TRUNC},
};

static bool		redir_right_fd(t_sh_redir const *r, t_sub right_str,
					uint32_t *dst)
{
	int				right_fd;

	if (g_sh_exec_redir[r->type].right_int)
	{
		if (ft_subto_uint(right_str, dst) < right_str.length)
			return (ASSERT(!"Invalid fd"), false);
		return (true);
	}
	if ((right_fd = open(right_str.str,
			g_sh_exec_redir[r->type].open_flags, 0644)) < 0)
		return (ASSERT(!"open fail"), false);
	*dst = right_fd;
	return (true);
}

static void		redir_close(t_vector const *redirs,
					uint32_t *right_fd, uint32_t count)
{
	uint32_t			i;

	i = 0;
	while (i < count)
	{
		if (g_sh_exec_redir[VGETC(t_sh_redir, *redirs, i).type].right_opened)
			close(right_fd[i]);
		i++;
	}
}

static bool		redir_open(t_sh_context *c, t_sh_redir_lst const *lst,
					uint32_t *left_fd, uint32_t *right_fd)
{
	uint32_t			i;
	t_sh_redir const	*r;
	t_str_list			right_str;
	t_sh_exec_text		exec_text;

	right_str = STR_LIST();
	i = 0;
	while (i < lst->redirs.length)
	{
		r = VECTOR_GET(lst->redirs, i);
		ft_str_list_clear(&right_str);
		exec_text = SH_EXEC_TEXT(&r->right_text);
		sh_exec_text(c, &exec_text, 0, &right_str);
		if (!((right_str.count == 1
				|| (ASSERT(!"ambiguous redir"), true))
			&& redir_right_fd(r, STR_LIST_SUB(right_str, 0), &right_fd[i])))
		{
			redir_close(&lst->redirs, right_fd, i);
			ft_str_list_clear(&right_str);
			return (false);
		}
		left_fd[i] = (r->left_fd < 0) ?
				g_sh_exec_redir[r->type].def_left : r->left_fd;
		i++;
	}
	ft_str_list_clear(&right_str);
	return (true);
}

// TODO: recover from a "fcntl fail" error
static void		redir_save(uint32_t *left_fd, uint32_t *saved_fd, uint32_t len)
{
	uint32_t		i;
	int				tmp;

	i = 0;
	while (i < len)
	{
		if ((tmp = fcntl(left_fd[i], F_DUPFD_CLOEXEC, 10)) < 0)
			HARD_ASSERT(!"fcntl fail");
		saved_fd[i] = tmp;
		i++;
	}
}

// TODO: recover from a "dup2 fail" error if 'saved_fd' is used
static void		redir_dup(uint32_t *left_fd, uint32_t *right_fd, uint32_t len)
{
	uint32_t		i;

	i = 0;
	while (i < len)
	{
		if (dup2(right_fd[i], left_fd[i]) < 0)
			HARD_ASSERT(!"dup2 fail");
		i++;
	}
}

bool			sh_exec_redir(t_sh_context *c, t_sh_redir_lst const *lst,
					uint32_t *saved_fd)
{
	uint32_t		left_fd[lst->redirs.length];
	uint32_t		right_fd[lst->redirs.length];

	if (!redir_open(c, lst, left_fd, right_fd))
		return (false);
	if (saved_fd != NULL)
		redir_save(left_fd, saved_fd, lst->redirs.length);
	redir_dup(left_fd, right_fd, lst->redirs.length);
	redir_close(&lst->redirs, right_fd, lst->redirs.length);
	return (true);
}

void			sh_exec_redir_restore(t_sh_redir_lst const *lst,
					uint32_t const *saved_fd)
{
	uint32_t			i;
	uint32_t			left_fd;
	t_sh_redir const	*r;

	i = 0;
	while (i < lst->redirs.length)
	{
		r = VECTOR_GET(lst->redirs, i);
		left_fd = (r->left_fd < 0) ?
				g_sh_exec_redir[r->type].def_left : r->left_fd;
		if (dup2(saved_fd[i], left_fd) < 0)
			HARD_ASSERT(!"dup2 fail");
		if (close(saved_fd[i]) < 0)
			ASSERT(!"close fail");
		i++;
	}
}
