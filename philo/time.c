/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:04:45 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 19:11:50 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_timer(void)
{
	struct timeval	time_now;

	gettimeofday(&time_now, NULL);
	return ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
}

void	destroy_all(t_philo *philo, t_all *var, pthread_t *th)
{
	destroy_mutex(philo);
	free_param(philo, philo->var->f, var);
	free(th);
}

unsigned long	current_time(t_philo *philo)
{
	return (get_timer() - philo->var->time);
}

void	philo_sleep(t_philo *philo)
{
	print(philo, "is sleeping");
	ft_usleep(philo, philo->var->time_sleep);
	philo->status = SLEEPING;
}

int	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->var->print);
	if (!philo_dead(philo))
		printf("%lu	%d %s\n", current_time(philo), philo->id, action);
	pthread_mutex_unlock(&philo->var->print);
	return (0);
}
