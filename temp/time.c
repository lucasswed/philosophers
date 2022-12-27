/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:04:45 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 00:35:15 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_timer(void)
{
	struct timeval	time_now;

	gettimeofday(&time_now, NULL);
	return ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
}

unsigned long	current_time(t_philo *philo)
{
	return (get_timer() - philo->var->time);
}

void	finnish_prog(t_philo *philo, t_all *var)
{
	destroy_mutex(philo);
	free_param(philo, philo->mutex, var);
}