/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systeme.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:14:40 by tguilbar          #+#    #+#             */
/*   Updated: 2020/11/09 09:55:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEME_H
# define SYSTEME_H

typedef struct	s_systeme
{
	struct timeval	init_time;
	pthread_t		*phil;
	pthread_mutex_t	*mutex_fork;
	pthread_mutex_t mutex_write;
	pthread_mutex_t mutex_goal;
	int				nb_fork;
	int				nb_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				goal;
}				t_systeme;

#endif
