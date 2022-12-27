/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:07 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 13:32:37 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_philo *philo, unsigned long time, char *action)
{
	pthread_mutex_lock(philo->print);
	if (!philo_dead(philo))
		printf("%lums	%d %s\n", time, philo->id, action);
	pthread_mutex_unlock(philo->print);
}

int	philo_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead);
	if (philo->var->finnish > 0)
	{
		pthread_mutex_unlock(philo->dead);
		return (1);
	}
	if (get_timer() - philo->last_meal
		> (unsigned long)philo->var->time_die)
	{
		philo->var->finnish++;
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
	pthread_mutex_lock(&philo->mutex[philo->id - 1]);
	print(philo, current_time(philo), "has taken a fork");
	pthread_mutex_lock(&philo->mutex[philo->id % philo->var->num_philo]);
	print(philo, current_time(philo), "has taken a fork");
	print(philo, current_time(philo), "is eating");
	philo->ate++;
	usleep(philo->var->time_eat * 1000);
	pthread_mutex_lock(philo->dead);
	philo->last_meal = get_timer();
	pthread_mutex_unlock(philo->dead);
	pthread_mutex_unlock(&philo->mutex[philo->id - 1]);
	pthread_mutex_unlock(&philo->mutex[philo->id]);
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
		usleep(10);
	pthread_mutex_lock(philo->dead);
	philo->last_meal = get_timer();
	pthread_mutex_unlock(philo->dead);
	while (philo->ate != philo->var->must_eat || !philo_dead(philo))
	{
		if (!philo_dead(philo))
			take_forks(philo);
		if (!philo_dead(philo))
			philo_sleep(philo);
		if (!philo_dead(philo))
			print(philo, current_time(philo), "is thinking");
	}
	printf("saiu %d\n", philo->id);
	return (NULL);
}

// void	finnish_func(t_philo *philo, t_all *var)
// {
// 	int	i;

// 	i = 0;
// 	while (1)
// 	{
// 		if (philo[i].var->total_ate == philo[i].var->num_philo)
// 		{
// 			philo->var->finnish++;
// 			return ;
// 		}
// 		pthread_mutex_lock(philo[i].dead);
// 		if (get_timer() - philo[i].last_meal
// 			> (unsigned long)var->time_die)
// 		{
// 			philo->var->finnish++;
// 			pthread_mutex_unlock(philo[i].dead);
// 			pthread_mutex_lock(philo->print);
// 			printf("%lums	%d died\n", current_time(philo), philo->id);
// 			pthread_mutex_unlock(philo->print);
// 			return ;
// 		}
// 		pthread_mutex_unlock(philo[i].dead);
// 		i = (i + 1) % var->num_philo;
// 		usleep(200);
// 	}
// }

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
		if (pthread_create(&th[i], NULL, routine, &(philo[i])))
			return (0);
		usleep(100);
	}
	i = -1;
	while (++i < philo->var->num_philo)
	{
		printf("i -> %d\n", i);
		pthread_join(th[i], NULL);
	}
	destroy_mutex(philo);
	free_param(philo, philo->mutex, var);
	free(th);
	return (0);
}
