/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:49:43 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 18:33:01 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
// creating the philo and start in parallel the routin of eating
void	start_simulation(t_table *tb)
{
	int	i;

	i = 0;
	tb->launch_time = get_timestamp();
	while (i < tb->num_eaters)
	{
		pthread_create(&tb->eaters[i].thread_id, NULL,
			&eating_routine, &tb->eaters[i]);
		i++;
	}
}

// checks each philo last meal time safely
// if any one from the philo did not eat in his time the simulation end
// stop the simulation when the number of meal reach the need
void	check_for_dead_philos(t_table *tb)
{
	int	i;

	i = 0;
	while (i < tb->num_eaters && !is_simulation_ended(tb))
	{
		pthread_mutex_lock(&tb->eaters[i].state_lock);
		if (get_timestamp() - tb->eaters[i].last_meal_time >= tb->meal_delay)
		{
			pthread_mutex_unlock(&tb->eaters[i].state_lock);
			report_action(&tb->eaters[i], "has died");
			set_simulation_end(tb);
			return ;
		}
		pthread_mutex_unlock(&tb->eaters[i].state_lock);
		i++;
	}
}

void	check_for_all_philos_fed(t_table *tb)
{
	if (tb->max_meals > 0)
	{
		pthread_mutex_lock(&tb->full_status);
		if (tb->nb_philo_eaten >= tb->num_eaters)
		{
			pthread_mutex_unlock(&tb->full_status);
			set_simulation_end(tb);
			return ;
		}
		pthread_mutex_unlock(&tb->full_status);
	}
}

void	monitor_eaters(t_table *tb)
{
	while (!is_simulation_ended(tb))
	{
		check_for_dead_philos(tb);
		check_for_all_philos_fed(tb);
		usleep(1000);
	}
}

// the main wait the philos to end befor exiting 
void	finish_simulation(t_table *tb)
{
	int	i;

	i = 0;
	while (i < tb->num_eaters)
		pthread_join(tb->eaters[i++].thread_id, NULL);
}
