/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaguillo <jaguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 17:43:00 by jaguillo          #+#    #+#             */
/*   Updated: 2017/02/26 17:52:21 by jaguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_UTILS_H
# define EDITOR_UTILS_H

# include "editor.h"

/*
** ========================================================================== **
** Move functions
** -
** Find the next stop after 'pos'
** If 'backward' is true, find the next stop decrementing from 'pos'
** -
** line				Next/prev line, same column
** linebound		End/begin of the current line
** bound			End/begin of the text
** word				After/before the next/prev word
** subword			After/before the next/prev word,
** 						stop at underscores, digits and uppercases
** char				Next/prev character
*/

uint32_t		editor_move_line(t_editor *e, uint32_t pos, bool backward);
uint32_t		editor_move_linebound(t_editor *e, uint32_t pos, bool backward);
uint32_t		editor_move_bound(t_editor *e, uint32_t pos, bool backward);
uint32_t		editor_move_word(t_editor *e, uint32_t pos, bool backward);
uint32_t		editor_move_subword(t_editor *e, uint32_t pos, bool backward);
uint32_t		editor_move_char(t_editor *e, uint32_t pos, bool backward);

#endif
