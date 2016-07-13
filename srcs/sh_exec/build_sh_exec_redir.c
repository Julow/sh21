/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_sh_exec_redir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/12 14:43:14 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/13 15:35:26 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_exec.h"
#include <fcntl.h>

static bool		redir_output(t_sh_exec_redir *redir, char *path, uint32_t oflags)
{
	int				fd;

	if ((fd = open(path, oflags, 0644)) < 0)
		return (ASSERT(!"Cannot open file"), false); // TODO: report error
	redir->fd.y = fd;
	redir->flags |= SH_REDIR_F_OPENED;
	return (true);
}

static bool		build_redir_output(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	// TODO: noclobber option
	// noclobber: add O_EXCL flag
	return (redir_output(redir, path, O_WRONLY | O_CREAT | O_TRUNC));
	(void)b;
}

static bool		build_redir_output_clobber(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	return (redir_output(redir, path, O_WRONLY | O_CREAT | O_TRUNC));
	(void)b;
}

static bool		build_redir_append(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	return (redir_output(redir, path, O_WRONLY | O_CREAT | O_APPEND));
	(void)b;
}

static bool		build_redir_input(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	int				fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (ASSERT(!"Cannot open file"), false);
	redir->fd.y = fd;
	redir->flags |= SH_REDIR_F_OPENED;
	return (true);
	(void)b;
}

static bool		build_redir_heredoc(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	ASSERT(!"input redir not implemented");
	return (false);
	(void)b;
	(void)redir;
	(void)path;
}

static bool		redir_close(t_sh_exec_builder *b, t_sh_exec_redir *redir)
{
	redir->flags |= SH_REDIR_F_CLOSE;
	return (true);
	(void)b;
}

static bool		build_redir_input_fd(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	t_sub const		left = ft_sub(path, 0, -1);
	uint32_t		fd;

	if (SUB_EQU(left, SUBC("-")))
		return (redir_close(b, redir));
	if (ft_subto_uint(left, &fd) != left.length)
		return (build_redir_input(b, redir, path));
	ASSERT(!"fd redir not implemented");
	return (false);
}

static bool		build_redir_output_fd(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	t_sub const		left = ft_sub(path, 0, -1);
	uint32_t		fd;

	if (SUB_EQU(left, SUBC("-")))
		return (redir_close(b, redir));
	if (ft_subto_uint(left, &fd) != left.length)
		return (build_redir_output(b, redir, path));
	ASSERT(!"fd redir not implemented");
	return (false);
}

static bool		build_redir_open(t_sh_exec_builder *b,
					t_sh_exec_redir *redir, char *path)
{
	ASSERT(!"input redir not implemented");
	return (false);
	(void)b;
	(void)redir;
	(void)path;
}

static struct
{
	bool			(*f)(t_sh_exec_builder *, t_sh_exec_redir *, char *);
	t_vec2u			def_fd;
} const			g_build_args_redirs[] = {
	[SH_REDIR_OUTPUT] = {&build_redir_output, VEC2U(1, 0)},
	[SH_REDIR_OUTPUT_CLOBBER] = {&build_redir_output_clobber, VEC2U(1, 0)},
	[SH_REDIR_APPEND] = {&build_redir_append, VEC2U(1, 0)},
	[SH_REDIR_INPUT] = {&build_redir_input, VEC2U(0, 0)},
	[SH_REDIR_HEREDOC] = {&build_redir_heredoc, VEC2U(0, 0)},
	[SH_REDIR_INPUT_FD] = {&build_redir_input_fd, VEC2U(0, 0)},
	[SH_REDIR_OUTPUT_FD] = {&build_redir_output_fd, VEC2U(1, 0)},
	[SH_REDIR_OPEN] = {&build_redir_open, VEC2U(1, 0)},
};

static bool		redir_left(t_sh_exec_builder *b, t_sh_redir_t type,
					t_sh_exec_redir *redir)
{
	t_sub const		last_word = SUB_FOR(DSTR_SUB(b->dst->buff), b->arg.offset);
	uint32_t		val;

	if (last_word.length == 0 || b->arg.quoted
		|| ft_subto_uint(last_word, &val) != last_word.length)
		return (true);
	redir->fd.x = val;
	b->dst->buff.length = b->arg.offset;
	return (true);
	(void)type;
}

static bool		redir_right(t_sh_exec_builder *b, t_sh_redir_t type,
					t_sh_exec_redir *redir)
{
	uint32_t const	arg_count = b->dst->args.length;
	t_sh_exec_arg	*last;

	while (b->dst->args.length <= arg_count)
	{
		if (b->token_i >= b->text->tokens.length)
		{
			build_sh_exec_push(b);
			if (b->dst->args.length <= arg_count)
				return (ASSERT(!"Incomplete redirection"), false); // TODO: report error
			break ;
		}
		if (!build_sh_exec_next(b))
			return (false);
	}
	if ((b->dst->args.length - arg_count) > 1)
		return (ASSERT(!"Ambiguous redirect"), false); // TODO: report error
	last = VECTOR_GET(b->dst->args, arg_count);
	if (!g_build_args_redirs[type].f(b, redir, b->dst->buff.str + last->offset))
		return (false);
	b->dst->buff.length = last->offset;
	b->dst->args.length--;
	b->arg = SH_EXEC_ARG(b->dst);
	return (true);
}

bool			build_args_redir(t_sh_exec_builder *b, t_sh_token const *token)
{
	t_sh_redir_t const	t = token->val.redir_type;
	t_sh_exec_redir		redir;

	redir = (t_sh_exec_redir){
		g_build_args_redirs[t].def_fd,
		0
	};
	if (!redir_left(b, t, &redir) || !redir_right(b, t, &redir))
		return (false);
	ft_vpush(&b->dst->redirs, &redir, 1);
	return (true);
}
