/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 11:50:36 by user42            #+#    #+#             */
/*   Updated: 2021/03/15 12:24:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

extern bool	g_end;
int			g_goal = 0;

void	eating(t_philosophe *entities)
{
	int time;

	if (g_end == true)
		return ;
	time = actual_time(*(entities->sys));
	put_msg(entities, "is eating\n");
	entities->death = time + entities->sys->time_to_die;
	ft_sleep(entities, time + entities->sys->time_to_eat);
	entities->nb_feeded++;
	if (entities->nb_feeded == entities->sys->goal)
	{
		pthread_mutex_lock(&entities->sys->mutex_goal);
		g_goal++;
		pthread_mutex_unlock(&entities->sys->mutex_goal);
	}
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
	t_systeme	*sys;
	int			nb_phil;

	sys = (t_systeme *)arg;
	nb_phil = sys->nb_phil;
	while (g_goal != nb_phil && g_end == false)
	{
		usleep(1000);
	}
	if (g_end)
		return (NULL);
	g_end = true;
	pthread_mutex_lock(&sys->mutex_write);
	write(1, "goal\n", 5);
	pthread_mutex_unlock(&sys->mutex_write);
	return (NULL);
}

void	*death_check(void *arg)
{
	t_philosophe	*entities;
	bool			*take;

	take = (bool *)(((void **)arg)[0]);
	entities = ((t_philosophe **)arg)[1];
	while (*take == false && g_end == false)
	{
		if (actual_time(*(entities->sys)) >= entities->death && *take == false
															&& g_end == false)
		{
			put_msg(entities, "died\n");
			g_end = true;
		}
		else
			usleep(1000);
	}
	return (NULL);
}

void	take_fork(int side, t_philosophe *entities)
{
	pthread_t	check;
	bool		take;
	void		*param[2];

	param[0] = &take;
	param[1] = entities;
	take = false;
	pthread_create(&check, NULL, death_check, (void*)param);
	pthread_mutex_lock(&entities->sys->mutex_fork[side]);
	take = true;
	pthread_join(check, NULL);
	if (g_end == true)
		return ;
	put_msg(entities, "has take a fork\n");
}
