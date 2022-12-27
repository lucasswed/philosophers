/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 23:27:24 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 00:42:35 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_die(t_philo *philo)
{
	pthread_mutex_lock(&(philo->dead));
	if (philo->var->finnish > 0)
	{
		pthread_mutex_unlock(&(philo->dead));
		return (0);
	}
	if (get_timer() - philo->last_meal
		> (unsigned long)philo->var->time_die)
	{
		philo->var->finnish++;
		pthread_mutex_unlock(&(philo->dead));
		print(philo, current_time(philo), "died");
		return (0);
	}
	pthread_mutex_unlock(&(philo->dead));
	return (1);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex[philo->id - 1]);
	print(philo, current_time(philo), "has taken a fork");
	pthread_mutex_lock(&philo->mutex[philo->id % philo->var->num_philo]);
	print(philo, current_time(philo), "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	print(philo, current_time(philo), "is eating");
	philo->ate++;
	usleep(philo->var->time_eat * 1000);
	pthread_mutex_unlock(&philo->mutex[philo->id - 1]);
	pthread_mutex_unlock(&philo->mutex[philo->id % philo->var->num_philo]);
}

void	philo_sleep(t_philo *philo)
{
	print(philo, current_time(philo), "is sleeping");
	usleep(philo->var->time_sleep * 1000);
}

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep((philo->var->time_eat * 1000) / 2);
	while (philo->ate < philo->var->must_eat && philo_die(philo))
	{
		if (philo_die(philo))
			take_forks(philo);
		if (philo_die(philo))
			philo_eat(philo);
		if (philo_die(philo))
			philo_sleep(philo);
		if (philo_die(philo))
			print(philo, current_time(philo), "is thinking");
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_all		*var;
	t_philo		*philo;

	if (ac < 5 || ac > 6)
		return (exit_error());
	var = malloc(sizeof(t_all));
	if (init_var(var, av))
		return (free_param(0, 0, var));
	philo = malloc(var->num_philo * sizeof(t_philo));
	fill_philo(philo, var);
	if (!init_threads(philo))
	{
		printf("Erro ao iniciar threads!");
		return (1);
	}
	finnish_prog(philo, var);
}