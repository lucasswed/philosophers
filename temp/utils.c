/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:44:37 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 00:15:30 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	exit_error(void)
{
	printf("Error! Invalid params. Please enter: num_of_philos\
 time_to_die time_to_eat time_to_sleep (num_of_times_each_philo_must_eat)\n");
	return (1);
}

void	print(t_philo *philo, unsigned long time, char *action)
{
	pthread_mutex_lock(&(philo->print));
	if (philo_die(philo))
		printf("%lums	%d %s\n", time, philo->id, action);
	pthread_mutex_unlock(&(philo->print));
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

		pthread_mutex_destroy(&philo->print);
		pthread_mutex_destroy(&philo->dead);
	i = -1;
	while (++i < philo->var->num_philo)
		pthread_mutex_destroy(&philo->mutex[i]);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	s;

	i = 0;
	res = 0;
	s = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
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
