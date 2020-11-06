/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 10:24:59 by tguilbar          #+#    #+#             */
/*   Updated: 2020/11/06 12:15:55 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

size_t	ft_strlen(char *msg)
{
	size_t size;

	size = 0;
	while (msg[size])
		size++;
	return (size);
}

char	*ft_itoa(int nb)
{
	char	*result;
	int		i;
	int		mem;

	mem = nb;
	i = 1;
	while (mem /= 10)
		i++;
	result = malloc(sizeof(char) * (i + 1));
	if (result == NULL)
		return (NULL);
	result[i] = '\0';
	while (i != 0)
	{
		i--;
		result[i] = (nb % 10 + 48);
		nb /= 10;
	}
	return (result);
}

char	*ft_strjoin(char *s1, char *s2, bool type)
{
	char	*result;
	int		i;
	int		j;

	i = ft_strlen(s1) + ft_strlen(s2);
	result = malloc(sizeof(char) * (i + 1));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	free(s1);
	j = 0;
	while (s2[j])
	{
		result[i + j] = s2[j];
		j++;
	}
	if (type)
		free(s2);
	result[i + j] = '\0';
	return (result);
}

void	put_msg(t_philosophe *entities, char *msg)
{
	char *result;

	result = ft_strjoin(ft_strjoin(ft_strjoin(ft_strjoin(ft_itoa(actual_time(
				*(entities->sys))), " ", false), ft_itoa(entities->id), true)
												, " ", false), msg, false);
	sem_wait(entities->sys->sem_write);
	write(1, result, ft_strlen(result));
	sem_post(entities->sys->sem_write);
	free(result);
}
