/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_syntax_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/09 21:32:29 by juloo             #+#    #+#             */
/*   Updated: 2016/02/10 00:33:39 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

struct			s_exec_data
{
	t_vector const	*color_scheme;
	t_callback		callback;
	t_vec2u			range;
};

static void const	*get_scheme(t_parse_frame *frame,
						t_vector const *color_scheme)
{
	uint32_t		i;
	t_color_scheme	*data;
	t_sub			*sub;

	while (frame != NULL)
	{
		sub = frame->data;
		i = 0;
		while (i < color_scheme->length)
		{
			data = VECTOR_GET(*color_scheme, i);
			if (ft_subfind(*sub, data->scope, 0) < sub->length)
				return (data->data);
			i++;
		}
		frame = frame->prev;
	}
	return (NULL);
}

/*

TODO:

move token_str and token_data into 'p'
ensure begin token is always set before call to begin function
ensure end token is set when parse_token return false

*/

bool			syntax_color_parser_begin(t_parse_data *p)
{
	t_parse_frame					frame;
	t_sub							token_str;
	void const						*token_data;
	t_sub							frame_scope;
	t_sub							token_scope;
	struct s_exec_data *const		data = p->env;

	frame_scope = ft_sub(p->frame->parser->data, 0, -1);
	p->frame->data = &frame_scope;
	frame = (t_parse_frame){NULL, NULL, p->frame};
	while (parse_token(p, &token_str, &token_data))
	{
		data->range.y = data->range.x + token_str.length;
		token_scope = ft_sub(token_data, 0, -1);
		frame.data = &token_scope;
		CALL(void, data->callback, data->range,
			get_scheme(&frame, data->color_scheme));
		data->range.x = data->range.y;
	}
	data->range.x += token_str.length;
	return (true);
}

void			exec_syntax_color(t_in *in, t_syntax_color const *syntax,
					t_vector const *color_scheme, t_callback callback)
{
	struct s_exec_data	data;

	data = (struct s_exec_data){color_scheme, callback, VEC2U(0, 0)};
	parse(in, syntax, &data);
}
