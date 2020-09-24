/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:46:27 by tguilbar          #+#    #+#             */
/*   Updated: 2020/05/19 12:14:41 by tguilbar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

bool g_end = false;

void	philosophe(t_philosophe *entities)
{
	while (true)
	{
		if (actual_time(*(entities->sys)) >= entities->death)
		{
			put_msg(*entities, "died\n");
			exit(-1);
		}
		put_msg(*entities, "is thinking\n");
		take_fork(entities);
		eating(entities);
		sem_post(entities->sys->sem_fork);
		sem_post(entities->sys->sem_fork);
		sleeping(*entities);
	}
	return ;
}

int		init(t_systeme *sys, t_philosophe *entities, pid_t **pid)
{
	entities->sys = sys;
	sys->nb_fork = (sys->nb_phil == 1) ? 2 : sys->nb_phil;
	*pid = malloc(sizeof(pid_t) * sys->nb_phil);
	sem_unlink("count_fork");
	sys->sem_fork = sem_open("count_fork", O_CREAT, 777, sys->nb_fork);
	sem_unlink("count_goal");
	sys->sem_goal = sem_open("count_goal", O_CREAT, 777, 0);
	if (*pid == NULL || sys->sem_fork == SEM_FAILED ||
													sys->sem_goal == SEM_FAILED)
	{
		free(*pid);
		sem_close(sys->sem_fork);
		sem_unlink("count_fork");
		sem_unlink("count_goal");
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
	t_philosophe	entities;
	t_systeme		sys;
	pid_t			*pid;
	pid_t			ret;
	int				i;

	if (take_param(ac, av, &sys) == -1)
		return (-1);
	if (init(&sys, &entities, &pid) == -1)
		return (-1);
	lunching_phil(&entities, pid);
	i = 0;
	ret = waitpid(-1, NULL, 0);
	while (i < sys.nb_phil)
	{
		if (ret != pid[i])
			kill(pid[i], 1);
		i++;
	}
	destructor(&entities, pid);
}
