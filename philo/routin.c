/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:47:33 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 18:33:24 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

// just for knowing what is the status of the philo
void	report_action(t_philo *ph, char *msg)
{
	pthread_mutex_lock(&ph->table->message_lock);
	if (!is_simulation_ended(ph->table))
	{
		printf("%lld %d %s\n",
			get_timestamp() - ph->table->launch_time, ph->id, msg);
	}
	pthread_mutex_unlock(&ph->table->message_lock);
}

// leaving the forks
void	release_forks(t_philo *ph)
{
	pthread_mutex_unlock(ph->left);
	pthread_mutex_unlock(ph->right);
}

// ensure consistent lock ordering to prevent deadlock
//acquire locks in order of memory address to prevent deadlock
void	acquire_forks(t_philo *ph)
{
	if (ph->left < ph->right)
	{
		pthread_mutex_lock(ph->left);
		report_action(ph, "has taken left chopstick");
		pthread_mutex_lock(ph->right);
		report_action(ph, "has taken right chopstick");
	}
	else
	{
		pthread_mutex_lock(ph->right);
		report_action(ph, "has taken right chopstick");
		pthread_mutex_lock(ph->left);
		report_action(ph, "has taken left chopstick");
	}
}

// take the left befor the right for aviod the data race
// then eat the meal and incriment the counter safley using lock
// last thing i left the forks
void	consume_meal(t_philo *ph)
{
	acquire_forks(ph);
	pthread_mutex_lock(&ph->state_lock);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->state_lock);
	report_action(ph, "eating");
	pause_execution(ph->table->dine_delay);
	pthread_mutex_lock(&ph->state_lock);
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->state_lock);
	release_forks(ph);
}

// because of the algo evrey philo first take the left fork
// so if the number is even they wana take some sleep for avioding 
// the deadlock (like infint loop waiting other)
// then they try to eat and the loop rest like that to fininsh 
// the number of meal or finishing the simulation (if philo die)
void	*eating_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_ended(ph->table))
	{
		report_action(ph, "thinking");
		pause_execution(ph->table->rest_delay);
		consume_meal(ph);
		if (ph->table->max_meals > 0 && ph->meals_eaten >= ph->table->max_meals)
		{
			pthread_mutex_lock(&ph->table->full_status);
			ph->table->nb_philo_eaten++;
			pthread_mutex_unlock(&ph->table->full_status);
			break ;
		}
	}
	return (NULL);
}
