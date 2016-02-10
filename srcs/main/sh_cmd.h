/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_cmd.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/10 14:39:51 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/10 14:41:17 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_CMD_H
# define SH_CMD_H

typedef struct s_sh_subst_expr	t_sh_subst_expr;
typedef struct s_sh_subst_math	t_sh_subst_math;
typedef struct s_sh_subst		t_sh_subst;
typedef struct s_sh_simple_cmd	t_sh_simple_cmd;
typedef struct s_sh_loop_cmd	t_sh_loop_cmd;
typedef struct s_sh_for_cmd		t_sh_for_cmd;
typedef struct s_sh_if_cmd		t_sh_if_cmd;
typedef struct s_sh_cmd			t_sh_cmd;
typedef enum e_sh_token			t_sh_token;
typedef enum e_sh_parser		t_sh_parser;

/*
** ========================================================================== **
** Shell command representation
*/

/*
** ========================================================================== **
** Substitutions
*/

struct		s_sh_subst_expr
{
	enum {
		SH_SUBST_EXPR_USE_DEF,
		SH_SUBST_EXPR_SET_DEF,
		SH_SUBST_EXPR_ISSET,
		SH_SUBST_EXPR_USE_ALT,
		SH_SUBST_EXPR_SUFFIX_PATTERN,
		SH_SUBST_EXPR_PREFIX_PATTERN,
		SH_SUBST_EXPR_F_COLON = (1 << 17),
		SH_SUBST_EXPR_F_LARGEST = (1 << 18),
	}			type;
	t_vec2u		param_range;
	t_vec2u		expr_range;
	t_vector	subst;
};

struct		s_sh_subst_math
{
	t_vec2u		math_range;
	t_vector	subst;
};

struct		s_sh_subst
{
	uint32_t	arg_index;
	enum {
		SH_SUBST_PARAM, // $PARAM, ${PARAM}
		SH_SUBST_STRLEN, // ${#PARAM}
		SH_SUBST_EXPR, // ${PARAM([:]?[-=?+]|[%#]{1,2})EXPR}
		SH_SUBST_MATH, // $(())
		SH_SUBST_CMD, // $(), ``
	}			type;
	t_vec2u		range;
	union {
		t_vec2u			param_range; // SH_SUBST_PARAM, SH_SUBST_STRLEN
		t_sh_subst_math	*math; // SH_SUBST_MATH
		t_sh_subst_expr	*expr; // SH_SUBST_EXPR
		t_sh_cmd		*cmd; // SH_SUBST_CMD
	}			val;
};

/*
** ========================================================================== **
** Shell command spec
*/

struct		s_sh_simple_cmd
{
	t_dstr		args;
	t_vector	arg_lengths;
	t_vector	subst;
};

struct		s_sh_loop_cmd
{
	enum {
		SH_CMD_LOOP_WHILE,
		SH_CMD_LOOP_UNTIL,
	}			type;
	t_sh_cmd	*cond;
	t_sh_cmd	*body;
};

struct		s_sh_for_cmd
{
	t_sh_simple_cmd	cmd;
	t_dstr			data;
	t_vector		words;
	t_sh_cmd		*body;
};

struct		s_sh_if_cmd
{
	t_sh_simple_cmd	cmd;
	t_sh_cmd		*body;
	enum {
		SH_IF_NEXT_ELIF,
		SH_IF_NEXT_ELSE,
	}				next_type;
	t_sh_if_cmd		*next;
};

/*
** ========================================================================== **
** Shell command
*/

struct		s_sh_cmd
{
	enum {
		SH_CMD_SIMPLE,
		SH_CMD_LOOP,
		SH_CMD_FORLOOP,
		SH_CMD_IF,
		SH_CMD_SUBSHELL,
	}			type;
	t_vector	redirs;
	union {
		t_sh_simple_cmd	cmd; // SH_CMD_SIMPLE
		t_sh_loop_cmd	loop; // SH_CMD_LOOP
		t_sh_for_cmd	cmd_for; // SH_CMD_FORLOOP
		t_sh_if_cmd		cmd_if; // SH_CMD_IF
		t_sh_cmd		*subshell; // SH_CMD_SUBSHELL
	}			val;
	enum {
		SH_NEXT_AND,
		SH_NEXT_OR,
		SH_NEXT_PIPE,
		SH_NEXT_NEWCMD,
	}			next_type;
};

#endif
