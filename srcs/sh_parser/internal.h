/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:25:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/11 14:44:30 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "ft/libft.h"
# include "ft/parser.h"
# include "sh/tokens.h"

struct			s_parse_sh_frame
{
	t_sh_cmd		*cmd;
	uint32_t		start_index;
};

t_sh_subst		*sh_simple_cmd_add_subst(t_sh_simple_cmd *s,
					enum e_sh_subst_type type, uint32_t length);

bool			sh_parse_cmd(t_parse_data *p);
bool			sh_parse_sub(t_parse_data *p);
// bool			sh_parse_expr(t_parse_data *p);
// bool			sh_parse_math(t_parse_data *p);
bool			sh_parse_string(t_parse_data *p);

#endif
