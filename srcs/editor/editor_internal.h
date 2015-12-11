/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:36:38 by jaguillo          #+#    #+#             */
/*   Updated: 2015/12/11 16:47:28 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_INTERNAL_H
# define EDITOR_INTERNAL_H

# include "editor.h"

typedef struct s_binding		t_binding;

struct 		s_binding
{
	t_key		key;
	bool		(*f)(t_editor*, t_key);
};

#endif
