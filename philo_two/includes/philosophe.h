/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophe.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 12:06:52 by tguilbar          #+#    #+#             */
/*   Updated: 2020/11/06 12:16:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHE_H
# define PHILOSOPHE_H

typedef struct	s_philosophe
{
	t_systeme	*sys;
	int			id;
	int			nb_feeded;
	int			death;
}				t_philosophe;

#endif
