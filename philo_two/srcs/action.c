/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 11:22:01 by tguilbar          #+#    #+#             */
/*   Updated: 2021/03/10 09:06:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

extern bool	g_end;
extern int	g_goal;
bool		g_beat = 1;

void	eating(t_philosophe *entities)
{
	int	time;

	if (g_end == true)
		return ;
	time = actual_time(*(entities->sys));
	put_msg(entities, "is eating\n");
	entities->death = time + entities->sys->time_to_die;
	ft_sleep(entities, time + entities->sys->time_to_eat);
	sem_post(entities->sys->sem_fork);
	sem_post(entities->sys->sem_fork);
	entities->nb_feeded++;
	if (entities->nb_feeded == entities->sys->goal)
		g_goal++;
}

void	sleeping(t_philosophe *entities)
{
	int time;

	if (g_end == true)
		return ;
	time = actual_time(*(entities->sys));
	put_msg(entities, "is sleeping\n");
	if (entities->death - time < entities->sys->time_to_sleep)
	{
		ft_sleep(entities, entities->death);
		put_msg(entities, "died\n");
		g_end = true;
	}
	else
		ft_sleep(entities, time + entities->sys->time_to_sleep);
}

void	*goal_check(void *arg)
{
	t_systeme	sys;
	int			nb_phil;

	sys = *((t_systeme *)arg);
	nb_phil = sys.nb_phil;
	while (g_goal != nb_phil && g_end == false)
	{
		usleep(1000);
	}
	if (g_end)
		return (NULL);
	g_end = true;
	sem_wait(sys.sem_write);
	write(1, "goal\n", 5);
	sem_post(sys.sem_write);
	return (NULL);
}

void	*death_check(void *arg)
{
	t_philosophe	*entities;
	bool			*take;

	take = ((bool **)arg)[0];
	entities = ((t_philosophe **)arg)[1];
	while (*take == false && g_end == false)
	{
		if (actual_time(*(entities->sys)) > entities->death &&
									*take == false && g_end == false)
		{
			put_msg(entities, "died\n");
			g_end = true;
			sem_post(entities->sys->sem_fork);
		}
		else
			usleep(1);
	}
	return (NULL);
}

void	take_fork(t_philosophe *entities)
{
	pthread_t	check;
	bool		take;
	void		*param[2];

	param[0] = &take;
	param[1] = entities;
	take = false;
	pthread_create(&check, NULL, death_check, (void*)param);
	while (g_beat != entities->id % 2 && g_end == false)
		usleep(1);
	sem_wait(entities->sys->sem_fork);
	put_msg(entities, "has take a fork\n");
	if (g_end == false)
	{
		sem_wait(entities->sys->sem_fork);
		put_msg(entities, "has take a fork\n");
		take = true;
	}
	pthread_join(check, NULL);
	if (g_end == true)
		return ;
	orga(entities->sys->nb_phil);
}
