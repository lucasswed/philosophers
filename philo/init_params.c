/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:38:00 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/21 17:48:25 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arg(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	init_var(t_all *var, char **av)
{
	if (check_arg(av))
		return (exit_error());
	var->total_ate = 0;
	var->time = get_timer();
	var->num_philo = ft_atoi(av[1]);
	var->time_die = ft_atoi(av[2]);
	var->time_eat = ft_atoi(av[3]);
	var->time_sleep = ft_atoi(av[4]);
	if (var->num_philo <= 0 || var->time_die <= 0\
	|| var->time_eat <= 0 || var->time_sleep <= 0)
		return (exit_error());
	if (av[5])
	{
		var->must_eat = ft_atoi(av[5]);
		if (var->must_eat <= 0)
			return (exit_error());
	}
	else
		var->must_eat = -1;
	return (0);
}

void	fill_philo(t_philo *philo, t_all *var,\
pthread_mutex_t *m, pthread_mutex_t *pr)
{
	int	i;

	i = 0;
	while (i < var->num_philo)
	{
		philo[i].ate = 0;
		philo[i].id = i + 1;
		philo[i].mutex = m;
		philo[i].print = pr;
		philo[i].var = var;
		i++;
	}
}

int	init_philo(t_philo *philo, t_all *var)
{
	pthread_mutex_t	*m;
	pthread_mutex_t	pr;
	int				i;

	i = 0;
	m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * var->num_philo);
	if (!m || !philo)
		return (free_param(philo, m, var));
	while (i < var->num_philo)
		if (pthread_mutex_init(&m[i++], 0))
			return (free_param(philo, m, var));
	if (pthread_mutex_init(&pr, 0))
		return (free_param(philo, m, var));
	fill_philo(philo, var, m, &pr);
	return (0);
}