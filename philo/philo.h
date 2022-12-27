/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:02:05 by lucas-ma          #+#    #+#             */
/*   Updated: 2022/12/27 19:12:04 by lucas-ma         ###   ########.fr       */
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

# define EATING		1
# define SLEEPING	2
# define THINKING	3

typedef struct s_fork
{
	int				*i;
	pthread_mutex_t	*c;
}	t_forks;

typedef struct s_all
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				finnish;
	pthread_mutex_t	print;
	pthread_mutex_t	dead;
	t_forks			*f;
	int				total_ate;
	unsigned long	time;
}				t_all;

typedef struct s_philo
{
	int				id;
	int				ate;
	int				status;
	t_all			*var;
	unsigned long	last_meal;
}				t_philo;

//-------------------------------MAIN.C---------------------------------//
int				philo_dead(t_philo *philo);

//----------------------------INIT_PARAMS.C-----------------------------//
int				init_var(t_all *var, char **av);
int				init_philo(t_philo *philo, t_all *var);

//-------------------------------UTILS.C--------------------------------//
int				exit_error(void);
int				ft_atoi(char *str);
void			destroy_mutex(t_philo *philo);
void			ft_usleep(t_philo *philo, int time);
int				free_param(t_philo *philo, t_forks *m, t_all *var);

//--------------------------------TIME.C--------------------------------//
unsigned long	get_timer(void);
void			philo_sleep(t_philo *philo);
unsigned long	current_time(t_philo *philo);
int				print(t_philo *philo, char *action);
void			destroy_all(t_philo *philo, t_all *var, pthread_t *th);

#endif