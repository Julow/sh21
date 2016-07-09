/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/09 17:11:23 by jaguillo          #+#    #+#             */
/*   Updated: 2016/07/09 18:10:16 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_sh_cmd		*sh_parse_cmd(t_in *in, t_sh_parse_err *err)
{
	t_sh_parse_data	p;
	bool			r;

	p = (t_sh_parse_data){PARSE_DATA(NULL, in), NULL, err};
	r = ft_parse(V(&p), load_sh_parser());
	D_PARSE_DATA(p.data);
	return (r ? p.cmd : NULL);
}

t_sh_cmd		*sh_parse_compound(t_in *in, t_sh_parse_err *err)
{
	t_sh_parse_data	p;
	t_sh_cmd		*cmd;
	t_sh_cmd		*first;
	bool			r;

	p = (t_sh_parse_data){PARSE_DATA(NULL, in), NULL, err};
	cmd = NULL;
	first = NULL;
	while (!PARSE_EOF(&p.data) && !PARSE_ERROR(&p.data)
		&& (r = ft_parse(V(&p), load_sh_parser())))
	{
		cmd = (first == NULL) ? (first = p.cmd) : (cmd->next = p.cmd);
		if (cmd->next_type == SH_NEXT_SEQ || cmd->next_type == SH_NEXT_ASYNC)
			return (first);
	}
	D_PARSE_DATA(p.data);
	if (!r && first != NULL)
		sh_destroy_cmd(first);
	return (r ? first : NULL);
}
