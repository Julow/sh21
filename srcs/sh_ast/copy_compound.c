/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_compound.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/10 11:38:19 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/11 14:34:53 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utils.h"
#include "sh/ast.h"

void			sh_copy_token_string(t_sh_token const *src, t_sh_token *dst)
{
	dst->token_len = src->token_len;
}

void			sh_copy_token_space(t_sh_token const *src, t_sh_token *dst)
{
	(void)src;
	(void)dst;
}

void			sh_copy_token_subshell(t_sh_token const *src, t_sh_token *dst)
{
	dst->cmd = NEW(t_sh_compound);
	sh_copy_compound(src->cmd, dst->cmd);
}

void			sh_copy_param_str_len(t_sh_param const *src, t_sh_param *dst)
{
	dst->type = src->type;
	dst->str_length = src->str_length;
}

void			sh_copy_param_pos(t_sh_param const *src, t_sh_param *dst)
{
	dst->type = src->type;
	dst->pos = src->pos;
}

void			sh_copy_param_special(t_sh_param const *src, t_sh_param *dst)
{
	dst->type = src->type;
	dst->special = src->special;
}

static void		(*const g_sh_copy_param[])(t_sh_param const *src,
					t_sh_param *dst) = {
	[SH_PARAM_STR] = &sh_copy_param_str_len,
	[SH_PARAM_LENGTH] = &sh_copy_param_str_len,
	[SH_PARAM_POS] = &sh_copy_param_pos,
	[SH_PARAM_SPECIAL] = &sh_copy_param_special,
};

void			sh_copy_token_param(t_sh_token const *src, t_sh_token *dst)
{
	g_sh_copy_param[src->param.type](&src->param, &dst->param);
}

static void		sh_copy_subst_param_str(t_sh_subst_param const *src,
					t_sh_subst_param *dst)
{
	sh_copy_text(&src->str, &dst->str);
}

static void		sh_copy_subst_param_end(t_sh_subst_param const *src,
					t_sh_subst_param *dst)
{
	(void)src;
	(void)dst;
}

static void		sh_copy_subst_param_repl(t_sh_subst_param const *src,
					t_sh_subst_param *dst)
{
	sh_copy_text(&src->repl[0], &dst->repl[1]);
	sh_copy_text(&src->repl[0], &dst->repl[1]);
}

static void		(*g_sh_copy_subst_param[])(t_sh_subst_param const *src,
					t_sh_subst_param *dst) = {
	[SH_SUBST_PARAM_DEF_NULL] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_DEF_UNSET] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_NULL] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_UNSET] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REPL_NULL] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REPL_UNSET] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_ERR_NULL] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_ERR_UNSET] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_UPPER_FIRST] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_UPPER] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_LOWER_FIRST] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_LOWER] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_INVCASE_FIRST] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_INVCASE] = &sh_copy_subst_param_end,
	[SH_SUBST_PARAM_REM_BEGIN] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REM_BEGIN_LONG] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REM_END] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REM_END_LONG] = &sh_copy_subst_param_str,
	[SH_SUBST_PARAM_REPL_FIRST] = &sh_copy_subst_param_repl,
	[SH_SUBST_PARAM_REPL_LAST] = &sh_copy_subst_param_repl,
	[SH_SUBST_PARAM_REPL_ALL] = &sh_copy_subst_param_repl,
};

void			sh_copy_token_subst_param(t_sh_token const *src,
					t_sh_token *dst)
{
	dst->subst_param = NEW(t_sh_subst_param);
	dst->subst_param->type = src->subst_param->type;
	g_sh_copy_param[src->subst_param->param.type](&src->subst_param->param,
			&dst->subst_param->param);
	g_sh_copy_subst_param[src->subst_param->type](src->subst_param,
			dst->subst_param);
}

static void		(*const g_sh_copy_token[])(t_sh_token const *src,
					t_sh_token *dst) = {
	[SH_T_STRING] = &sh_copy_token_string,
	[SH_T_SPACE] = &sh_copy_token_space,
	[SH_T_SUBSHELL] = &sh_copy_token_subshell,
	[SH_T_PARAM] = &sh_copy_token_param,
	[SH_T_SUBST_PARAM] = &sh_copy_token_subst_param,
};

void			sh_copy_text(t_sh_text const *src, t_sh_text *dst)
{
	t_sh_token const	*t_src;
	t_sh_token			*t_dst;
	uint32_t			i;

	*dst = SH_TEXT();
	ft_dstradd(&dst->text, DSTR_SUB(src->text));
	t_dst = ft_vpush(&dst->tokens, NULL, src->tokens.length);
	i = 0;
	while (i < src->tokens.length)
	{
		t_src = VECTOR_GET(src->tokens, i);
		t_dst[i].type = t_src->type;
		g_sh_copy_token[t_src->type & ~SH_F_T_QUOTED](t_src, t_dst + i);
		i++;
	}
}

void			sh_copy_redir(t_sh_redir const *src, t_sh_redir *dst)
{
	dst->left_fd = src->left_fd;
	dst->type = src->type;
	sh_copy_text(&src->right_text, &dst->right_text);
}

void			sh_copy_redir_lst(t_sh_redir_lst const *src,
					t_sh_redir_lst *dst)
{
	t_sh_redir			*r_dst;
	uint32_t			i;

	dst->redirs = VECTOR(t_sh_redir);
	r_dst = ft_vpush(&dst->redirs, NULL, src->redirs.length);
	i = 0;
	while (i < src->redirs.length)
	{
		sh_copy_redir(VECTOR_GET(src->redirs, i), r_dst + i);
		i++;
	}
}

static void		sh_copy_cmd_simple(t_sh_cmd const *src, t_sh_cmd *dst)
{
	sh_copy_text(&src->simple.text, &dst->simple.text);
}

static void		sh_copy_cmd_subshell(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->subshell = NEW(t_sh_compound);
	sh_copy_compound(src->subshell, dst->subshell);
}

static void		sh_copy_if(t_sh_if const *src, t_sh_if *dst)
{
	sh_copy_compound(&src->cond, &dst->cond);
	sh_copy_compound(&src->body, &dst->body);
	dst->else_clause = NULL;
	if (src->else_clause != NULL)
	{
		dst->else_clause = NEW(t_sh_else);
		dst->else_clause->type = src->else_clause->type;
		if (src->else_clause->type == SH_ELSE_ELIF)
			sh_copy_if(&src->else_clause->elif_clause,
				&dst->else_clause->elif_clause);
		else
			sh_copy_compound(&src->else_clause->else_clause,
				&dst->else_clause->else_clause);
	}
}

static void		sh_copy_cmd_if_clause(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->if_clause = NEW(t_sh_if);
	sh_copy_if(src->if_clause, dst->if_clause);
}

static void		sh_copy_cmd_for_clause(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->for_clause = MALLOC(sizeof(t_sh_for) + src->for_clause->var.length);
	dst->for_clause->var = SUB_DST(ENDOF(dst->for_clause),
			src->for_clause->var);
	sh_copy_text(&src->for_clause->data, &dst->for_clause->data);
	sh_copy_compound(&src->for_clause->body, &dst->for_clause->body);
}

static void		sh_copy_cmd_while_clause(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->while_clause = NEW(t_sh_while);
	sh_copy_compound(&src->while_clause->cond, &dst->while_clause->cond);
	sh_copy_compound(&src->while_clause->body, &dst->while_clause->body);
}

static void		sh_copy_cmd_rec_clause(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->rec = NEW(t_sh_cmd);
	sh_copy_cmd(src->rec, dst->rec);
}

static void		sh_copy_cmd_bracket_clause(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->bracket_clause = NEW(t_sh_compound);
	sh_copy_compound(src->bracket_clause, dst->bracket_clause);
}

static void		sh_copy_cmd_function_def(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->function_def = MALLOC(sizeof(t_sh_func_def)
				+ src->function_def->name.length);
	dst->function_def->name = SUB_DST(ENDOF(dst->function_def),
			src->function_def->name);
	sh_copy_cmd(&src->function_def->body, &dst->function_def->body);
}

static void		(*const g_sh_copy_cmd[])(t_sh_cmd const *src, t_sh_cmd *dst) = {
	[SH_CMD_SIMPLE] = &sh_copy_cmd_simple,
	[SH_CMD_SUBSHELL] = &sh_copy_cmd_subshell,
	[SH_CMD_IF_CLAUSE] = &sh_copy_cmd_if_clause,
	[SH_CMD_FOR_CLAUSE] = &sh_copy_cmd_for_clause,
	[SH_CMD_WHILE_CLAUSE] = &sh_copy_cmd_while_clause,
	[SH_CMD_UNTIL_CLAUSE] = &sh_copy_cmd_while_clause,
	[SH_CMD_TIME_CLAUSE] = &sh_copy_cmd_rec_clause,
	[SH_CMD_NOT_CLAUSE] = &sh_copy_cmd_rec_clause,
	[SH_CMD_BRACKET_CLAUSE] = &sh_copy_cmd_bracket_clause,
	[SH_CMD_FUNCTION_DEF] = &sh_copy_cmd_function_def,
};

void			sh_copy_cmd(t_sh_cmd const *src, t_sh_cmd *dst)
{
	dst->type = src->type;
	sh_copy_redir_lst(&src->redirs, &dst->redirs);
	g_sh_copy_cmd[src->type](src, dst);
}

void			sh_copy_pipeline(t_sh_pipeline const *src, t_sh_pipeline *dst)
{
	sh_copy_cmd(&src->cmd, &dst->cmd);
	dst->next = NULL;
	if (src->next != NULL)
	{
		dst->next = NEW(t_sh_pipeline);
		sh_copy_pipeline(src->next, dst->next);
	}
}

void			sh_copy_list(t_sh_list const *src, t_sh_list *dst)
{
	sh_copy_pipeline(&src->pipeline, &dst->pipeline);
	dst->next = NULL;
	if (src->next != NULL)
	{
		dst->next = NEW(t_sh_list_next);
		dst->next->type = src->next->type;
		sh_copy_list(&src->next->next, &dst->next->next);
	}
}

void			sh_copy_compound(t_sh_compound const *src, t_sh_compound *dst)
{
	sh_copy_list(&src->list, &dst->list);
	dst->flags = src->flags;
	dst->next = NULL;
	if (src->next != NULL)
	{
		dst->next = NEW(t_sh_compound);
		sh_copy_compound(src->next, dst->next);
	}
}
