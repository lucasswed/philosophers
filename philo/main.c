/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:07 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/23 12:31:59 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_philo *philo, unsigned long time, char *action)
{
	if (!philo_dead(philo))
	{
		pthread_mutex_lock(philo->print);
		printf("%lums	%d %s\n", time, philo->id, action);
		pthread_mutex_unlock(philo->print);
	}
}

int	philo_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead);
	if (philo->var->died > 0)
	{
		pthread_mutex_unlock(philo->dead);
		return (1);
	}
	if ((unsigned long)philo->var->time_die < \
get_timer() - philo->last_meal)
	{
		philo->var->died++;
		pthread_mutex_unlock(philo->dead);
		pthread_mutex_lock(philo->print);
		printf("%lums	%d died\n", current_time(philo), philo->id);
		pthread_mutex_unlock(philo->print);
		return (1);
	}
	pthread_mutex_unlock(philo->dead);
	return (0);
}

void	take_forks(t_philo	*philo)
{
	print(philo, current_time(philo), "has taken a fork");
	pthread_mutex_lock(&philo->mutex[philo->id - 1]);
	pthread_mutex_lock(&philo->mutex[philo->id]);
	print(philo, current_time(philo), "has taken a fork");
	print(philo, current_time(philo), "is eating");
	philo->ate++;
	usleep(philo->var->time_eat * 1000);
	philo->last_meal = get_timer();
	pthread_mutex_unlock(&philo->mutex[philo->id - 1]);
	pthread_mutex_unlock(&philo->mutex[philo->id]);
}

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep((philo->var->time_die * 1000) / 2);
	philo->last_meal = get_timer();
	while (!philo_dead(philo) || philo->var->must_eat != philo->ate)
	{
		if (!philo_dead(philo))
			take_forks(philo);
		if (!philo_dead(philo))
		{
			print(philo, current_time(philo), "is sleeping");
			usleep(philo->var->time_sleep * 1000);
		}
		if (!philo_dead(philo))
			print(philo, current_time(philo), "is thinking");
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	int			i;
	t_all		*var;
	t_philo		*philo;
	pthread_t	*th;

	if (ac < 5 || ac > 6)
		return (exit_error());
	var = malloc(sizeof(t_all));
	if (init_var(var, av))
		return (free_param(0, 0, var));
	philo = malloc(var->num_philo * sizeof(t_philo));
	if (init_philo(philo, var))
		return (1);
	th = malloc(sizeof(pthread_t) * var->num_philo);
	i = -1;
	while (++i < philo->var->num_philo)
	{
		pthread_create(&th[i], NULL, routine, &(philo[i]));
		usleep(100);
	}
	i = -1;
	while (++i < philo->var->num_philo)
		pthread_join(th[i], NULL);
	destroy_mutex(philo);
	free_param(philo, philo->mutex, var);
	free(th);
	return (0);
}
