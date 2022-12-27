/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:07 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 19:12:44 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->var->dead);
	if (philo->var->finnish > 0)
	{
		pthread_mutex_unlock(&philo->var->dead);
		return (1);
	}
	if (get_timer() - philo->last_meal
		> (unsigned long)philo->var->time_die)
	{
		philo->var->finnish++;
		pthread_mutex_unlock(&philo->var->dead);
		printf("%lu	%d died\n", current_time(philo), philo->id);
		return (1);
	}
	pthread_mutex_unlock(&philo->var->dead);
	return (0);
}

static void	take_forks(t_philo	*p)
{
	while (!philo_dead(p))
	{
		pthread_mutex_lock(p->var->f[p->id - 1].c);
		if (*(p->var->f[p->id - 1].i) == 0)
		{
			*(p->var->f[p->id - 1].i) = 1;
			pthread_mutex_unlock(p->var->f[p->id - 1].c);
			break ;
		}
		pthread_mutex_unlock(p->var->f[p->id - 1].c);
	}
	print(p, "has taken a fork");
	while (!philo_dead(p))
	{
		pthread_mutex_lock(p->var->f[p->id % p->var->num_philo].c);
		if (*(p->var->f[p->id % p->var->num_philo].i) == 0)
		{
			*(p->var->f[p->id % p->var->num_philo].i) = 1;
			pthread_mutex_unlock(p->var->f[p->id % p->var->num_philo].c);
			break ;
		}
		pthread_mutex_unlock(p->var->f[p->id % p->var->num_philo].c);
	}
	print(p, "has taken a fork");
}

static void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	print(philo, "is eating");
	philo->ate++;
	ft_usleep(philo, philo->var->time_eat);
	pthread_mutex_lock(philo->var->f[philo->id - 1].c);
	*(philo->var->f[philo->id - 1].i) = 0;
	pthread_mutex_unlock(philo->var->f[philo->id - 1].c);
	pthread_mutex_lock(philo->var->f[philo->id % philo->var->num_philo].c);
	*(philo->var->f[philo->id % philo->var->num_philo].i) = 0;
	pthread_mutex_unlock(philo->var->f[philo->id % philo->var->num_philo].c);
	pthread_mutex_lock(&philo->var->dead);
	philo->last_meal = get_timer();
	pthread_mutex_unlock(&philo->var->dead);
	philo->status = EATING;
}

static void	*routine(void *p)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)p;
	if (philo->var->num_philo >= 100)
		i = (philo->var->must_eat * 1000) / 2;
	else
		i = 1000;
	if (philo->id % 2 == 0)
		usleep(i);
	philo->last_meal = get_timer();
	while (philo->ate != philo->var->must_eat && !philo_dead(philo))
	{
		if (!philo_dead(philo) && philo->status == THINKING)
			philo_eat(philo);
		if (philo->ate == philo->var->must_eat)
			break ;
		if (!philo_dead(philo) && philo->status == EATING)
			philo_sleep(philo);
		if (!philo_dead(philo) && philo->status == SLEEPING)
			philo->status = print(philo, "is thinking") + THINKING;
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
		if (pthread_create(&th[i], NULL, routine, &(philo[i])))
			return (0);
	i = -1;
	while (++i < philo->var->num_philo)
		pthread_join(th[i], NULL);
	destroy_all(philo, var, th);
	return (0);
}
