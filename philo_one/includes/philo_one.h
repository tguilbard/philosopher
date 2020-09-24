/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tguilbar <tguilbar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 13:45:07 by tguilbar          #+#    #+#             */
/*   Updated: 2020/05/17 12:22:52 by tguilbar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include "philo_include.h"

int		actual_time(t_systeme sys);
void	eating(t_philosophe *entities);
void	sleeping(t_philosophe entities);
void	take_fork(int side, t_philosophe *entities);
void	*goal_check(void *arg);
void	put_msg(t_philosophe entities, char *msg);
int		ft_atoi(char *str);
void	ft_sleep(int time);
void	destructor(t_philosophe *entities);
void	lunching_phil(t_philosophe *entities);
void	*philosophe(void *arg);

#endif
