/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:38:00 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 00:40:26 by lucas-ma         ###   ########.fr       */
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

void	fill_philo(t_philo *philo, t_all *var)
{
	int				i;

	i = 0;
	while (i < var->num_philo)
	{
		philo[i].ate = 0;
		philo[i].id = i + 1;
		philo[i].var = var;
		i++;
	}
}

int	init_threads(t_philo *philo)
{
	int				i;
	pthread_t		*th;
	
	th = malloc(sizeof(pthread_t) * philo->var->num_philo);
	philo->mutex = malloc(sizeof(pthread_mutex_t) * philo->var->num_philo);
	if (!th || !(philo->mutex))
		return (0);
	if (pthread_mutex_init(&(philo->dead), NULL)
		|| pthread_mutex_init(&(philo->print), NULL))
		return (0);
	i = -1;
	while (++i < philo->var->num_philo)
		if (pthread_mutex_init(&(philo->mutex[i]), NULL))
			return (0);
	i = -1;
	while (++i < philo->var->num_philo)
		if (pthread_create(&th[i], NULL, routine, &(philo[i])))
			return (0);
	i = -1;
	while (++i < philo->var->num_philo)
		pthread_join(th[i], NULL);
	free(th);
	return (1);
}
