/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systeme.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:14:40 by tguilbar          #+#    #+#             */
/*   Updated: 2020/11/17 14:34:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEME_H
# define SYSTEME_H

typedef struct	s_systeme
{
	struct timeval	init_time;
	sem_t			*sem_fork;
	sem_t			*sem_goal;
	sem_t			*sem_write;
	sem_t			*sem_count;
	sem_t			*sem_even;
	sem_t			*sem_uneven;
	pid_t			*pid;
	int				nb_fork;
	int				nb_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				goal;
}				t_systeme;

#endif
