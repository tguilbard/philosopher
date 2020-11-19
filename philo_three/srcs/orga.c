/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orga.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <tguilbar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 14:19:02 by user42            #+#    #+#             */
/*   Updated: 2020/11/17 15:21:28 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	fill_sem(sem_t *sem, int i)
{
	int j;

	j = 0;
	while (j < i)
	{
		sem_post(sem);
		j++;
	}
}

/*
**	tab[0] = tab[0]; tab[1] = tab[1]
*/

void	*orga(void *arg)
{
	t_philosophe	*entities;
	int				tab[2];
	bool			beat;

	entities = (t_philosophe *)arg;
	tab[1] = entities->sys->nb_phil;
	beat = 1;
	while (true)
	{
		sem_wait(entities->sys->sem_count);
		tab[0]++;
		if (tab[0] == (tab[1] / 2) + (tab[1] % 2) && beat == 1)
		{
			fill_sem(entities->sys->sem_even, tab[1] / 2);
			beat = 0;
			tab[0] = 0;
		}
		else if (tab[0] == tab[1] / 2 && beat == 0)
		{
			fill_sem(entities->sys->sem_uneven, (tab[1] / 2) + (tab[1] % 2));
			beat = 1;
			tab[0] = 0;
		}
	}
	return (NULL);
}
