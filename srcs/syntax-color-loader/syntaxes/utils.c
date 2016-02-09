/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 00:41:22 by juloo             #+#    #+#             */
/*   Updated: 2016/02/09 12:45:53 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

t_syntax_color_def const	g_syntax_color_utils = SYNTAX_COLOR("utils", "",
	.def = VECTORC(((t_parser_def[]){

		PARSER_DEF("string", "string", NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T("\"", "end", .end=true),
				PARSER_T("\\\"", "escaped.quote"),
			),
		),

		PARSER_DEF("allow-blanks", "blanks", NULL,
			.tokens = PARSER_DEF_T(
				PARSER_T(" ", "blank.space"),
				PARSER_T("\t", "blank.tab"),
				PARSER_T("\n", "blank.newline"),
			),
		),

	})),
);
