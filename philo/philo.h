/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 18:35:22 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 19:28:38 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include<stdio.h>
# include<stdlib.h>
# include<unistd.h>
# include<pthread.h>
# include<sys/time.h>

typedef struct s_shared_table	t_table;

//for protecting the philo state
typedef struct s_philosopher
{
	int				id;
	long long		last_meal_time;
	int				meals_eaten;
	pthread_t		thread_id;
	t_table			*table;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	state_lock;
}	t_philo;

//array of mutex
//for protecting the printing from each philo
//mutex to protect is_ended variable
//number of philos
//counter of philo how has eat all the max meals
//array of philos
typedef struct s_shared_table{
	pthread_mutex_t	*chopsticks;
	pthread_mutex_t	message_lock;
	pthread_mutex_t	full_status;
	pthread_mutex_t	end_lock;
	long long		launch_time;
	int				max_meals;
	int				num_eaters;
	int				meal_delay;
	int				dine_delay;
	int				rest_delay;
	int				nb_philo_eaten;
	int				is_ended;
	t_philo			*eaters;
}	t_table;

int			string_to_int(const char *input);
long long	get_timestamp(void);
void		pause_execution(long long msec);
int			is_simulation_ended(t_table *table);
void		set_simulation_end(t_table *table);
void		report_action(t_philo *ph, char *msg);
void		acquire_forks(t_philo *ph);
void		release_forks(t_philo *ph);
void		consume_meal(t_philo *ph);
void		*eating_routine(void *arg);
void		setup_dining_table(t_table *tb, char **av);
void		setup_dining_table2(t_table *tb);
void		start_simulation(t_table *tb);
void		check_for_dead_philos(t_table *tb);
void		check_for_all_philos_fed(t_table *tb);
void		monitor_eaters(t_table *tb);
void		finish_simulation(t_table *tb);
void		cleanup_dining_table(t_table *tb);

#endif
