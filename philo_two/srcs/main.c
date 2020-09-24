/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:46:27 by tguilbar          #+#    #+#             */
/*   Updated: 2020/05/22 12:53:28 by tguilbar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

bool	g_end = false;
int		g_goal = 0;

void	*philosophe(void *arg)
{
	t_philosophe	*entities;

	entities = (t_philosophe*)arg;
	while (true)
	{
		if (g_end == true)
			return (NULL);
		if (actual_time(*(entities->sys)) >= entities->death)
		{
			g_end = true;
			put_msg(*entities, "died\n");
			return (NULL);
		}
		put_msg(*entities, "is thinking\n");
		take_fork(entities);
		eating(entities);
		sem_post(entities->sys->sem_fork);
		sem_post(entities->sys->sem_fork);
		sleeping(*entities);
	}
	return (NULL);
}

int		init(t_systeme *sys, t_philosophe **entities)
{
	int i;

	*entities = malloc(sizeof(t_philosophe) * sys->nb_phil);
	if (*entities == NULL)
		return (-1);
	i = 0;
	while (i < sys->nb_phil)
		((*entities)[i++]).sys = sys;
	sys->nb_fork = (sys->nb_phil == 1) ? 2 : sys->nb_phil;
	sys->phil = malloc(sizeof(pthread_t) * sys->nb_phil);
	sem_unlink("count_fork");
	sys->sem_fork = sem_open("count_fork", O_CREAT, 777, sys->nb_fork);
	if (sys->phil == NULL || sys->sem_fork == SEM_FAILED)
	{
		free(*entities);
		free(sys->phil);
		sem_close(sys->sem_fork);
		sem_unlink("count_fork");
		return (-1);
	}
	gettimeofday(&(sys->init_time), NULL);
	return (0);
}

int		take_param(int ac, char **av, t_systeme *sys)
{
	if (ac == 5 || ac == 6)
	{
		sys->nb_phil = ft_atoi(av[1]);
		sys->time_to_die = ft_atoi(av[2]);
		sys->time_to_eat = ft_atoi(av[3]);
		sys->time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
			sys->goal = ft_atoi(av[5]);
		else
			sys->goal = -1;
	}
	else
		return (-1);
	return (0);
}

int		main(int ac, char **av)
{
	t_philosophe	*entities;
	t_systeme		sys;
	int				i;

	if (take_param(ac, av, &sys) == -1)
		return (-1);
	if (init(&sys, &entities) == -1)
		return (-1);
	lunching_phil(entities);
	i = 0;
	while (i < sys.nb_phil)
	{
		pthread_join(sys.phil[i], NULL);
		i++;
	}
	destructor(entities);
}
