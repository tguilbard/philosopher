/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 12:11:16 by tguilbar          #+#    #+#             */
/*   Updated: 2020/05/22 13:03:36 by tguilbar         ###   ########.fr       */
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

void	destructor(t_philosophe *entities, pid_t *pid)
{
	free(pid);
	sem_close(entities->sys->sem_fork);
	sem_unlink("count_fork");
	sem_unlink("count_goal");
}

void	lunching_phil(t_philosophe *entities, pid_t *pid)
{
	t_systeme	sys;
	pthread_t	check;
	void		*param[2];
	int			i;

	sys = *(entities->sys);
	param[0] = &sys;
	param[1] = pid;
	i = 0;
	pthread_create(&check, NULL, goal_check, (void *)param);
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
