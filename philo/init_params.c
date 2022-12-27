/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:38:00 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 19:01:07 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_arg(char **av)
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
	var->finnish = 0;
	var->time = get_timer();
	var->num_philo = ft_atoi(av[1]);
	var->time_die = ft_atoi(av[2]);
	var->time_eat = ft_atoi(av[3]);
	var->time_sleep = ft_atoi(av[4]);
	if (var->num_philo <= 0 || var->time_die <= 0
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

static t_forks	*init_forks(t_philo *philo, t_all *var)
{
	int				i;
	int				*in_use;
	pthread_mutex_t	*c;
	t_forks			*m;

	i = -1;
	m = malloc(sizeof(t_forks) * var->num_philo);
	c = malloc(sizeof(pthread_mutex_t) * var->num_philo);
	in_use = malloc(sizeof(int) * var->num_philo);
	if (!m || !c || !in_use)
		return (NULL);
	while (++i < var->num_philo)
	{
		m[i].c = &c[i];
		m[i].i = &in_use[i];
		if (pthread_mutex_init(&c[i], NULL))
		{
			free_param(philo, m, var);
			return (NULL);
		}
	}
	return (m);
}

static int	fill_philo(t_philo *philo, t_all *var,
					pthread_mutex_t dead, pthread_mutex_t pr)
{
	int				i;
	t_forks			*m;

	i = -1;
	var->print = pr;
	var->dead = dead;
	m = init_forks(philo, var);
	if (!m)
		return (1);
	while (++i < var->num_philo)
	{
		philo[i].ate = 0;
		philo[i].id = i + 1;
		philo[i].var = var;
		philo[i].status = THINKING;
	}
	var->f = m;
	return (0);
}

int	init_philo(t_philo *philo, t_all *var)
{
	pthread_mutex_t	pr;
	pthread_mutex_t	dead;

	if (!philo || !var)
		return (free_param(philo, 0, var));
	if (pthread_mutex_init(&pr, NULL)
		|| pthread_mutex_init(&dead, NULL))
		return (free_param(philo, 0, var));
	if (fill_philo(philo, var, dead, pr))
	{
		if (var->f)
		{
			if (var->f->c)
				free(var->f->c);
			if (var->f->i)
				free(var->f->i);
			free (var->f);
			return (free_param(philo, 0, var));
		}
	}
	return (0);
}
