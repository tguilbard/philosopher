/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 11:22:01 by tguilbar          #+#    #+#             */
/*   Updated: 2020/11/17 14:57:44 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

bool		g_take = false;
extern bool g_end;

void	eating(t_philosophe *entities)
{
	int time;

	put_msg(entities, "is eating\n");
	time = actual_time(*(entities->sys));
	entities->death = time + entities->sys->time_to_die;
	ft_sleep(entities, time + entities->sys->time_to_eat);
	entities->nb_feeded++;
	if (entities->nb_feeded == entities->sys->goal)
		sem_post(entities->sys->sem_goal);
}

void	sleeping(t_philosophe *entities)
{
	int time;

	time = actual_time(*(entities->sys));
	put_msg(entities, "is sleeping\n");
	if (entities->death - time < entities->sys->time_to_sleep)
	{
		ft_sleep(entities, entities->death);
		put_msg(entities, "died\n");
		exit(-1);
	}
	else
		ft_sleep(entities, time + entities->sys->time_to_sleep);
}

void	*goal_check(void *arg)
{
	t_systeme	*sys;
	pid_t		*pid;
	int			i;

	sys = ((t_systeme **)arg)[0];
	pid = ((pid_t **)arg)[1];
	i = 0;
	while (i < sys->nb_phil)
	{
		sem_wait(sys->sem_goal);
		i++;
	}
	i = 0;
	while (i < sys->nb_phil)
	{
		kill(pid[i], 1);
		i++;
	}
	exit(-1);
}

void	*death_check(void *arg)
{
	t_philosophe	*entities;

	entities = (t_philosophe *)arg;
	while (g_take == false)
	{
		if (actual_time(*(entities->sys)) >= entities->death && g_take == false)
		{
			put_msg(entities, "died\n");
			sem_post(entities->sys->sem_fork);
			g_end = true;
			return (NULL);
		}
		else
			usleep(1000);
	}
	return (NULL);
}

void	take_fork(t_philosophe *entities)
{
	pthread_t	check;

	g_take = false;
	pthread_create(&check, NULL, death_check, (void*)entities);
	pthread_detach(check);
	if ((entities->id % 2) == 0)
		sem_wait(entities->sys->sem_even);
	else
		sem_wait(entities->sys->sem_uneven);
	sem_post(entities->sys->sem_count);
	sem_wait(entities->sys->sem_fork);
	if (g_end == true)
		exit(-1);
	put_msg(entities, "has take a fork\n");
	sem_wait(entities->sys->sem_fork);
	if (g_end == true)
		exit(-1);
	g_take = true;
	put_msg(entities, "has take a fork\n");
}
