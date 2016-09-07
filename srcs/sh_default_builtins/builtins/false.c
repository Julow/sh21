/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   false.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 17:58:09 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 18:09:13 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_default_builtins.h"

int				sh_builtin_false(t_sh_context *c, void *data, t_argv args)
{
	return (1);
	(void)c;
	(void)data;
	(void)args;
}
