/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_destroy_cmd.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juloo <juloo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 18:54:55 by juloo             #+#    #+#             */
/*   Updated: 2016/07/24 18:55:36 by juloo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_DESTROY_CMD_H
# define P_DESTROY_CMD_H

# include "ft/libft.h"
# include "sh/ast.h"

void			sh_destroy_if_clause(t_sh_if *c);
void			sh_destroy_for_clause(t_sh_for *c);
void			sh_destroy_while_clause(t_sh_while *c);

#endif
