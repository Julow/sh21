/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put_bind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 17:56:38 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 17:57:48 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_default_bindings.h"

bool			editor_put_bind(t_editor_put_bind *b, t_editor *e)
{
	editor_put(e, b->text, false);
	return (true);
}
