/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 12:11:16 by tguilbar          #+#    #+#             */
/*   Updated: 2021/02/25 13:02:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

extern bool	g_end;
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

void	destructor(t_philosophe *entities)
{
	free(entities->sys->phil);
	sem_close(entities->sys->sem_fork);
	sem_unlink("count_fork");
	sem_close(entities->sys->sem_write);
	sem_unlink("secure_output");
	free(entities);
}

void	ft_sleep(t_philosophe *entities, int end)
{
	while (actual_time(*(entities->sys)) < end)
		usleep(2);
}

void	orga(int nb_phil)
{
	static int	count = 0;

	count++;
	if (count == (nb_phil / 2) + (nb_phil % 2) && g_beat == 1)
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

void	lunching_phil(t_philosophe *entities)
{
	t_systeme	*sys;
	pthread_t	check;
	int			i;

	sys = (entities[0]).sys;
	i = 0;
	pthread_create(&check, NULL, goal_check, (void*)sys);
	pthread_detach(check);
	while (i < sys->nb_phil)
	{
		(entities[i]).id = i + 1;
		(entities[i]).nb_feeded = 0;
		(entities[i]).death = actual_time(*sys) +
										(entities[i]).sys->time_to_die;
		pthread_create(&((sys->phil)[i]), NULL,
											philosophe, (void*)&(entities[i]));
		i++;
	}
}
