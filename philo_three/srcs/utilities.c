/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 12:11:16 by tguilbar          #+#    #+#             */
/*   Updated: 2021/02/25 12:18:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		actual_time(t_systeme sys)
{
	struct timeval	time;
	int				result;

	gettimeofday(&time, NULL);
	result = ((time.tv_usec - sys.init_time.tv_usec) / 1000);
	result += ((time.tv_sec - sys.init_time.tv_sec) * 1000);
	return (result);
}

void	ft_sleep(t_philosophe *entities, int end)
{
	while (actual_time(*(entities->sys)) < end)
		usleep(1);
}

void	destructor(t_philosophe *entities, pid_t *pid)
{
	free(pid);
	sem_close(entities->sys->sem_fork);
	sem_unlink("count_fork");
	sem_close(entities->sys->sem_goal);
	sem_unlink("count_goal");
	sem_close(entities->sys->sem_write);
	sem_unlink("secure_output");
	sem_close(entities->sys->sem_count);
	sem_unlink("count_orga");
	sem_close(entities->sys->sem_even);
	sem_unlink("even");
	sem_close(entities->sys->sem_uneven);
	sem_unlink("uneven");
}

void	lunching_phil(t_philosophe *entities, pid_t *pid)
{
	t_systeme	sys;
	pthread_t	check;
	int			i;

	sys = *(entities->sys);
	i = 0;
	pthread_create(&check, NULL, goal_check, (void *)&sys);
	pthread_detach(check);
	entities->nb_feeded = 0;
	entities->death = actual_time(sys) + (entities[i]).sys->time_to_die;
	while (i < sys.nb_phil)
	{
		entities->id = i + 1;
		if ((pid[i] = fork()) == -1)
			exit(-1);
		if (pid[i] == 0)
		{
			philosophe(entities);
			exit(0);
		}
		i++;
	}
}
