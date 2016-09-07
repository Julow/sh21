/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_builtins.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:10:10 by jaguillo          #+#    #+#             */
/*   Updated: 2016/09/07 16:12:31 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULT_BUILTINS_H
# define DEFAULT_BUILTINS_H

# include "ft/libft.h"
# include "sh/context.h"

/*
** ========================================================================== **
** Default builtins
*/

void			sh_init_default_builtins(t_sh_context *c);

#endif
