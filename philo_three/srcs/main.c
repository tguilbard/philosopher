/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 11:46:27 by tguilbar          #+#    #+#             */
/*   Updated: 2021/03/15 11:52:31 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	philosophe(t_philosophe *entities)
{
	while (true)
	{
		if (actual_time(*(entities->sys)) >= entities->death)
		{
			put_msg(entities, "died\n");
			destructor(entities, NULL);
			exit(-1);
		}
		put_msg(entities, "is thinking\n");
		take_fork(entities);
		eating(entities);
		sem_post(entities->sys->sem_fork);
		sem_post(entities->sys->sem_fork);
		sleeping(entities);
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
	sem_unlink("secure_output");
	sys->sem_write = sem_open("secure_output", O_CREAT, 777, 1);
	sem_unlink("count_orga");
	sys->sem_count = sem_open("count_orga", O_CREAT, 777, 0);
	sem_unlink("even");
	sys->sem_even = sem_open("even", O_CREAT, 777, 0);
	sem_unlink("uneven");
	sys->sem_uneven = sem_open("uneven", O_CREAT, 777, (sys->nb_phil / 2) +
															(sys->nb_phil % 2));
	if (*pid == NULL || sys->sem_fork == S_FAILED || sys->sem_even == S_FAILED
		|| sys->sem_goal == S_FAILED || sys->sem_write == S_FAILED ||
		sys->sem_count == S_FAILED || sys->sem_uneven == S_FAILED)
	{
		destructor(entities, *pid);
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
		if (sys->nb_phil < 1 || sys->time_to_die < 60 || sys->time_to_eat < 60
													|| sys->time_to_sleep < 60)
		{
			write(2, "Wrong Input\n", 12);
			return (-1);
		}
	}
	else
	{
		write(2, "Wrong Input\n", 12);
		return (-1);
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_philosophe	entities;
	t_systeme		sys;
	pthread_t		th;
	pid_t			ret;
	int				i;

	if (take_param(ac, av, &sys) == -1)
		return (-1);
	if (init(&sys, &entities, &sys.pid) == -1)
		return (-1);
	sys.end = false;
	if (0 != pthread_create(&th, NULL, orga, (void *)&entities))
		return (-1);
	pthread_detach(th);
	lunching_phil(&entities, sys.pid);
	i = 0;
	ret = waitpid(-1, NULL, 0);
	sys.end = true;
	while (i < sys.nb_phil)
	{
		if (ret != (sys.pid)[i])
			kill((sys.pid)[i], 1);
		i++;
	}
	destructor(&entities, sys.pid);
}
