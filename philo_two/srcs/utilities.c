/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 12:11:16 by tguilbar          #+#    #+#             */
/*   Updated: 2020/10/28 10:54:46 by tguilbar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

extern bool	g_end;

int		actual_time(t_systeme sys)
{
	struct timeval	time;
	int				result;

	gettimeofday(&time, NULL);
	result = ((time.tv_usec - sys.init_time.tv_usec) / 1000);
	result += ((time.tv_sec - sys.init_time.tv_sec) * 1000);
	return (result);
}

void	ft_sleep(int time)
{
	int i;

	i = time / 1000;
	while (i > 0 && g_end == false)
	{
		usleep(1000 * 1000);
		i--;
	}
	if (g_end == false)
		usleep((time % 1000) * 1000);
}

void	destructor(t_philosophe *entities)
{
	free(entities->sys->phil);
	sem_close(entities->sys->sem_fork);
	sem_unlink("count_fork");
	sem_close(entities->sys->sem_write);
	sem_unlink("secure_output");
	free(entities);
}

void	lunching_phil(t_philosophe *entities)
{
	t_systeme	sys;
	pthread_t	check;
	int			i;

	sys = *(entities->sys);
	i = 0;
	pthread_create(&check, NULL, goal_check, (void*)&(sys.nb_phil));
	pthread_detach(check);
	while (i < sys.nb_phil)
	{
		(entities[i]).id = i + 1;
		(entities[i]).nb_feeded = 0;
		(entities[i]).death = actual_time(sys) + (entities[i]).sys->time_to_die;
		pthread_create(&((sys.phil)[i]), NULL,
											philosophe, (void*)&(entities[i]));
		i++;
	}
}
