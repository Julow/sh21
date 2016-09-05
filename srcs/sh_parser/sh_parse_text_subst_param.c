/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_subst_param.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 12:16:01 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/05 17:27:24 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"
#include <stdlib.h>

static void		parse_subst_param(t_sh_parser *p,
					t_sh_text *dst_text, t_sh_param *dst)
{
	t_sub				param_str;

	if (SH_T(p)->type == SH(PARAM))
	{
		param_str = SUB_FOR(p->l.t.token_str, SH_T(p)->param_prefix);
		*dst = SH_PARAM(STR, .str_length=param_str.length);
		ft_dstradd(&dst_text->text, param_str);
	}
	else if (SH_T(p)->type == SH(PARAM_LENGTH))
	{
		param_str = SUB_FOR(p->l.t.token_str, SH_T(p)->param_prefix);
		*dst = SH_PARAM(LENGTH, .str_length=param_str.length);
		ft_dstradd(&dst_text->text, param_str);
	}
	else if (SH_T(p)->type == SH(PARAM_POS))
	{
		param_str = SUB_FOR(p->l.t.token_str, SH_T(p)->param_prefix);
		*dst = SH_PARAM(POS, .pos=param_str.str[0] - '0');
	}
	else if (SH_T(p)->type == SH(PARAM_SPECIAL))
	{
		*dst = SH_PARAM(SPECIAL, .special=SH_T(p)->param_special);
	}
	else
		ASSERT(false);
}

static bool		parse_param_text(t_sh_parser *p, t_sh_text *dst)
{
	while (true)
	{
		if (!ft_lexer_next(&p->l))
			return (sh_parse_error(p, SH_E_EOF));
		if (g_sh_parse_text[SH_T(p)->type] != NULL)
		{
			if (!g_sh_parse_text[SH_T(p)->type](p, dst, false))
				return (false);
		}
		else if (SH_T(p)->type == SH(SUBST_PARAM_END)
			|| SH_T(p)->type == SH(SUBST_PARAM_OP))
			break ;
	}
	return (true);
}

static bool		sh_parse_text_subst_param_str(t_sh_parser *p,
					t_sh_subst_param *dst)
{
	dst->str = SH_TEXT();
	return (parse_param_text(p, &dst->str));
}

static bool		sh_parse_text_subst_param_end(t_sh_parser *p,
					t_sh_subst_param *dst)
{
	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, SH_E_EOF));
	if (SH_T(p) == NULL)
		return (sh_parse_error(p, SH_E_ERROR));
	return (true);
	(void)dst;
}

static bool		sh_parse_text_subst_param_repl(t_sh_parser *p,
					t_sh_subst_param *dst)
{
	dst->repl[0] = SH_TEXT();
	dst->repl[1] = SH_TEXT();
	if (!parse_param_text(p, &dst->repl[0]))
		return (false);
	if (SH_T(p)->type != SH(SUBST_PARAM_OP))
		return (true);
	if (!parse_param_text(p, &dst->repl[1]))
		return (false);
	if (SH_T(p)->type != SH(SUBST_PARAM_END))
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	return (true);
}

static bool		(*g_sh_parse_text_subst_param[])(t_sh_parser *p,
					t_sh_subst_param *dst) = {
	[SH_SUBST_PARAM_DEF_NULL] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_DEF_UNSET] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_NULL] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_ASSIGN_UNSET] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REPL_NULL] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REPL_UNSET] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_ERR_NULL] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_ERR_UNSET] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_UPPER_FIRST] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_UPPER] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_LOWER_FIRST] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_LOWER] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_INVCASE_FIRST] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_INVCASE] = &sh_parse_text_subst_param_end,
	[SH_SUBST_PARAM_REM_BEGIN] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REM_BEGIN_LONG] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REM_END] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REM_END_LONG] = &sh_parse_text_subst_param_str,
	[SH_SUBST_PARAM_REPL_FIRST] = &sh_parse_text_subst_param_repl,
	[SH_SUBST_PARAM_REPL_LAST] = &sh_parse_text_subst_param_repl,
	[SH_SUBST_PARAM_REPL_ALL] = &sh_parse_text_subst_param_repl,
};

static bool		parse_text_subst_param_op(t_sh_parser *p, t_sh_text *dst,
					bool quoted, t_sh_param const *param_param)
{
	t_sh_subst_param	*param;

	if (SH_T(p)->type != SH(SUBST_PARAM_OP))
		return (sh_parse_error(p, SH_E_UNEXPECTED));
	param = NEW(t_sh_subst_param);
	param->param = *param_param;
	param->type = SH_T(p)->subst_param_op;
	if (!g_sh_parse_text_subst_param[param->type](p, param))
	{
		free(param);
		return (false);
	}
	sh_text_push(dst, SUB0(),
		SH_TOKEN(SUBST_PARAM, .subst_param=param), quoted);
	return (true);
}

bool			sh_parse_text_subst_param(t_sh_parser *p,
					t_sh_text *dst, bool quoted)
{
	t_lexer_state const		*prev;
	t_sh_param				param;

	ft_lexer_push(&p->l, &prev);
	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, p->l.eof ? SH_E_EOF : SH_E_ERROR));
	parse_subst_param(p, dst, &param);
	ft_lexer_pop(&p->l, prev);
	ft_lexer_push(&p->l, &prev);
	if (!ft_lexer_next(&p->l))
		return (sh_parse_error(p, p->l.eof ? SH_E_EOF : SH_E_ERROR));
	if (SH_T(p)->type == SH(SUBST_PARAM_END))
		sh_text_push(dst, SUB0(), SH_TOKEN(PARAM, .param=param), quoted);
	else
	{
		ft_lexer_pop(&p->l, prev);
		ft_lexer_push(&p->l, &prev);
		if (!parse_text_subst_param_op(p, dst, quoted, &param))
			return (false);
	}
	ft_lexer_pop(&p->l, prev);
	return (true);
}
