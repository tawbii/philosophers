/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:52:12 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 18:31:09 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"
// just to setup the table and the philos to start
void	setup_dining_table(t_table *tb, char **av)
{
	int	i;

	i = 0;
	tb->num_eaters = string_to_int(av[1]);
	tb->meal_delay = string_to_int(av[2]);
	tb->dine_delay = string_to_int(av[3]);
	tb->rest_delay = string_to_int(av[4]);
	if (av[5])
		tb->max_meals = string_to_int(av[5]);
	else
		tb->max_meals = -1;
	tb->is_ended = 0;
	tb->nb_philo_eaten = 0;
	pthread_mutex_init(&tb->message_lock, NULL);
	pthread_mutex_init(&tb->full_status, NULL);
	pthread_mutex_init(&tb->end_lock, NULL);
	tb->chopsticks = malloc(sizeof(pthread_mutex_t) * tb->num_eaters);
	tb->eaters = malloc(sizeof(t_philo) * tb->num_eaters);
	while (i < tb->num_eaters)
	{
		pthread_mutex_init(&tb->chopsticks[i], NULL);
		i++;
	}
}

//is a helper function to the first setup
void	setup_dining_table2(t_table *tb)
{
	int	i;

	i = 0;
	while (i < tb->num_eaters)
	{
		tb->eaters[i].id = i + 1;
		tb->eaters[i].last_meal_time = get_timestamp();
		tb->eaters[i].meals_eaten = 0;
		tb->eaters[i].table = tb;
		tb->eaters[i].left = &tb->chopsticks[i];
		tb->eaters[i].right = &tb->chopsticks[(i + 1) % tb->num_eaters];
		pthread_mutex_init(&tb->eaters[i].state_lock, NULL);
		i++;
	}
}

// destroy shared mutex and the mutex of the chopstiks 
// and free the allocate arrays of mutex 
void	cleanup_dining_table(t_table *tb)
{
	int	i;

	i = 0;
	while (i < tb->num_eaters)
	{
		pthread_mutex_destroy(&tb->chopsticks[i]);
		pthread_mutex_destroy(&tb->eaters[i].state_lock);
		i++;
	}
	pthread_mutex_destroy(&tb->message_lock);
	pthread_mutex_destroy(&tb->full_status);
	pthread_mutex_destroy(&tb->end_lock);
	free(tb->chopsticks);
	free(tb->eaters);
}
