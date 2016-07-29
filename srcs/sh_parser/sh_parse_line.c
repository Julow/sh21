/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/28 14:51:34 by juloo             #+#    #+#             */
/*   Updated: 2016/07/29 00:57:54 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/parser.h"
#include "sh/parser.h"

#include "p_sh_parser.h"

bool			sh_parse_line(t_in *in, t_sh_compound *dst, t_sh_parse_err *err)
{
	t_sh_parse_data	p_data;
	t_parse_data	p;
	bool			r;

	p_data = SH_PARSE_DATA(err);
	p = PARSE_DATA(&p_data, in);
	r = ft_parse(&p, sh_load_parser(SUBC("sh-compound")), dst);
	D_PARSE_DATA(p);
	return (r);
}
