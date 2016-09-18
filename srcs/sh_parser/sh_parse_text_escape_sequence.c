/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_parse_text_escape_sequence.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 01:19:30 by juloo             #+#    #+#             */
/*   Updated: 2016/09/18 11:32:57 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_sh_parser.h"

static uint32_t	ft_subto_uint_octal(t_sub sub, uint32_t *dst)
{
	uint32_t		i;

	i = 0;
	*dst = 0;
	while (i < sub.length && sub.str[i] >= '0' && sub.str[i] <= '7')
		*dst = *dst * 010 + sub.str[i++] - '0';
	return (i);
}

static uint32_t	ft_subto_uint_hex(t_sub sub, uint32_t *dst)
{
	uint32_t		i;
	uint32_t		c;

	i = 0;
	*dst = 0;
	while (i < sub.length)
	{
		if (sub.str[i] >= '0' && sub.str[i] <= '9')
			c = sub.str[i] - '0';
		else if (sub.str[i] >= 'a' && sub.str[i] <= 'f')
			c = sub.str[i] - 'a' + 0xa;
		else if (sub.str[i] >= 'A' && sub.str[i] <= 'F')
			c = sub.str[i] - 'A' + 0xa;
		else
			break ;
		*dst = *dst * 0x10 + c;
		i++;
	}
	return (i);
}

static bool		get_escaped_value(t_sh_parser *p, uint32_t *value)
{
	t_sh_parse_token const	*t;
	t_sub					str;
	uint32_t				len;

	if (!ft_tokenize_ahead(&p->t, &str, V(&t)) || t->type != SH(TEXT))
		return (false);
	len = 0;
	if (SH_T(p)->escape_sequence == SH(ESCAPE_OCTAL))
		len = ft_subto_uint_octal(SUB(str.str, MIN(str.length, 3)), value);
	else if (SH_T(p)->escape_sequence == SH(ESCAPE_HEXA))
		len = ft_subto_uint_hex(SUB(str.str, MIN(str.length, 2)), value);
	else if (SH_T(p)->escape_sequence == SH(ESCAPE_CONTROL))
	{
		*value = str.str[0] - 'a' + 1;
		len = (str.str[0] >= 'a' && str.str[0] <= 'z') ? 1 : 0;
	}
	else
		ASSERT(false);
	if (len == 0)
		return (false);
	ft_tokenize(&p->t);
	// ft_tokenizer_inject(&p->t, SUB_FOR(str, len));
	return (true);
}

bool			sh_parse_text_escape_sequence(t_sh_parser *p, // TODO: rework sh_parse_text_escape_sequence
					t_sh_text *dst, bool quoted)
{
	uint32_t				value;
	char					char_value;

	if (!get_escaped_value(p, &value))
	{
		sh_text_push_string(dst, SUBC("\\"), true);
	}
	else
	{
		char_value = (char)(uint8_t)value;
		sh_text_push_string(dst, SUB(&char_value, 1), true);
	}
	return (true);
	(void)quoted;
}
