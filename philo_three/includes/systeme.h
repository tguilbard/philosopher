/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systeme.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:14:40 by tguilbar          #+#    #+#             */
/*   Updated: 2020/05/19 12:04:35 by tguilbar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEME_H
# define SYSTEME_H

typedef struct	s_systeme
{
	struct timeval	init_time;
	sem_t			*sem_fork;
	sem_t			*sem_goal;
	int				nb_fork;
	int				nb_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				goal;
}				t_systeme;

#endif