/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:05 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/21 17:46:50 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct	s_all
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				total_ate;
	unsigned long	time;
}				t_all;

typedef struct	s_philo
{
	int				id;
	int				ate;
	t_all			*var;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	unsigned long	last_meal;
}				t_philo;

//----------------------------INIT_PARAMS.C-----------------------------//
int		init_var(t_all *var, char **av);
int		init_philo(t_philo *philo, t_all *var);

//-------------------------------UTILS.C--------------------------------//
int		exit_error(void);
int		ft_atoi(char *str);
void	destroy_mutex(t_philo *philo);
int		free_param(t_philo *philo, pthread_mutex_t *m, t_all *var);

//--------------------------------TIME.C--------------------------------//
unsigned long	get_timer(void);
unsigned long	current_time(t_philo *philo);

#endif