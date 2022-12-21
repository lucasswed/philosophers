/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:44:37 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/21 17:55:24 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	exit_error(void)
{
	printf("Error! Invalid params. Please enter: num_of_philos\
 time_to_die time_to_eat time_to_sleep (num_of_times_each_philo_must_eat)\n");
	return (1);
}

int	free_param(t_philo *philo, pthread_mutex_t *m, t_all *var)
{
	if (philo)
		free(philo);
	if (m)
		free(m);
	if (var)
		free(var);
	return (1);
}

void	destroy_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->var->num_philo)
		pthread_mutex_destroy(&philo->mutex[i++]);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	s;

	i = 0;
	res = 0;
	s = 1;
	while((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		i++;
	}
	return (res * s);
}