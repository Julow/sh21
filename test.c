/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/08 15:22:19 by jaguillo          #+#    #+#             */
/*   Updated: 2016/01/10 00:23:52 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lol.h"
#include <stdlib.h>

#define MDR				(xd + 5)

#define MDR(LOL)		(LOL * 8 + MDR)

// test

/*
** lol
*/

int				main(void)
{
	int				mdr;
	int8_t uint8_t uint_fast8_t int_fast8_t
	int16_t uint16_t uint_fast16_t int_fast16_t
	int32_t uint32_t uint_fast32_t int_fast32_t
	int64_t uint64_t uint_fast64_t int_fast64_t
	char bool short int long void unsigned
	const while if else do for typedef struct
	false true NULL break continue
	t_sub t_dstr t_lol lol_t pid_t time_t

	mdr = 1;
	printf("lol $MDR\n");
	printf("%s\n");
	printf("%ts\n");
	printf("%*.5lls\n");
	printf("%10.*lls\n");
	printf("%10c\n");
	printf("%%\n");
	printf("%^mM+ '#-%\n");
	printf("?w\n");
	printf("?+w\n");
	printf("?*w\n", 0.5f, 0f, 0.f);
	printf("'mdr lol'\n");
	printf("?'?'??(#??'#'{?+w})?*[!i=-]?(?*d,?*d)|?[?*+]??(&?*d)?[.aludnswb^$]|('?=*.')|(\"?=*.\")|([?=*.])\n");
	SYNTAX_T("\\???(#??'#'{?+w})?*[!i=-]?(?*d,?*d)|?[?*+]??(&?*d)?[.aludnswb^$]|('?-*.')|(\"?-*.\")|([?-*.])|({??[&:]?-*.})|(?#{BRACE}((?*(?!*'('??{BRACE}))))", "format.regex"),
		"\033[39m""\033[31m""\033[32m""\033[33m""\033[34m""\033[35m""\033[36m"
		"\033[90m""\033[30m""\033[97m""\033[91m""\033[92m""\033[93m""\033[94m"
		"\033[95m""\033[96m""\033[37m""\033[49m""\033[107m""\033[40m""\033[100m"
		"\033[106m""\033[41m""\033[42m""\033[43m""\033[44m""\033[45m""\033[46m"
		"\033[47m""\033[101m""\033[102m""\033[103m""\033[104m""\033[105m"
	return (mdr);
}
