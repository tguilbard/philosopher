/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 12:11:16 by tguilbar          #+#    #+#             */
/*   Updated: 2021/03/09 13:35:39 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

extern bool g_end;
extern bool g_beat;

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

void	destructor(t_philosophe *entities)
{
	free(entities->sys->phil);
	free(entities->sys->mutex_fork);
	free(entities);
}

void	lunching_phil(t_philosophe *entities)
{
	t_systeme	sys;
	pthread_t	check;
	int			i;

	sys = *(entities->sys);
	i = 0;
	pthread_create(&check, NULL, goal_check, (void*)entities->sys);
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

void	orga(int nb_phil)
{
	static int count = 0;

	count++;
	if (count == (nb_phil / 2) + (nb_phil % 2) && g_beat == 1 && nb_phil > 1)
	{
		g_beat = 0;
		count = 0;
	}
	else if (count == nb_phil / 2 && g_beat == 0)
	{
		g_beat = 1;
		count = 0;
	}
}
