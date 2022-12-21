/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:07 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/21 21:05:13 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_philo *philo, unsigned long time, char *action)
{
	pthread_mutex_lock(philo->print);
	printf("%lums	%d %s\n", time, philo->id, action);
	pthread_mutex_unlock(philo->print);
}

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(10);
	philo->last_meal = get_timer();
	while (1)
	{
		pthread_mutex_lock(&philo->mutex[philo->id - 1]);
		print(philo, current_time(philo), "has taken a fork");
		pthread_mutex_lock(&philo->mutex[philo->id % philo->var->num_philo]);
		print(philo, current_time(philo), "has taken a fork");
		print(philo, current_time(philo), "is eating");
		philo->ate++;
		if (philo->ate == philo->var->must_eat)
			philo->var->total_ate++;
		usleep(philo->var->time_eat * 1000);
		philo->last_meal = get_timer();
		pthread_mutex_unlock(&philo->mutex[philo->id - 1]);
		pthread_mutex_unlock(&philo->mutex[philo->id % philo->var->num_philo]);
		print(philo, current_time(philo), "is sleeping");
		usleep(philo->var->time_sleep * 1000);
		print(philo, current_time(philo), "is thinking");
	}
	return (NULL);
}

void	check_finnish(t_philo *philo, t_all *var)
{
	int	i;

	i = 0;
	while (1)
	{
		if (philo[i].var->total_ate == philo[i].var->num_philo)
		{
			destroy_mutex(philo);
			free_param(philo, philo->mutex, var);
			return ;
		}
		if (get_timer() - philo[i].last_meal > (unsigned long)var->time_die)
		{
			usleep(100);
			pthread_mutex_lock(philo->print);
			printf("%lums	%d died\n", current_time(philo), philo->id);
			destroy_mutex(philo);
			free_param(philo, philo->mutex, var);
			return ;
		}
		i = (i + 1) % var->num_philo;
		usleep(200);
	}
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
	i = 0;
	while (i < philo->var->num_philo)
	{
		pthread_create(th, NULL, routine, &(philo[i]));
		usleep(100);
		i++;
	}
	check_finnish(philo, var);
	free(th);
	return (0);
}