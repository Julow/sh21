/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 14:25:44 by jaguillo          #+#    #+#             */
/*   Updated: 2016/02/14 01:33:29 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

# include "ft/libft.h"
# include "ft/parser.h"
# include "sh/tokens.h"

bool			sh_parse_text(t_parse_data *p, t_sh_text *text);

bool			sh_parse_cmd(t_parse_data *p);
bool			sh_parse_sub(t_parse_data *p);
// bool			sh_parse_expr(t_parse_data *p);
// bool			sh_parse_math(t_parse_data *p);
bool			sh_parse_string(t_parse_data *p);

bool			sh_parse_ignore(t_parse_data *p);

#endif
