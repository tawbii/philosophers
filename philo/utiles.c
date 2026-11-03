/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:44:38 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 18:35:04 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

// just for parssing
int	string_to_int(const char *input)
{
	int	res;
	int	neg;

	res = 0;
	neg = 1;
	while (*input == ' ' || (*input >= 9 && *input <= 13))
		input++;
	if (*input == '+' || *input == '-')
	{
		if (*input == '-')
			neg = -1;
		input++;
	}
	while (*input >= '0' && *input <= '9')
		res = res * 10 + (*input++ - '0');
	return (res * neg);
}

// geting the real time to the starting the simulation
long long	get_timestamp(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long)now.tv_sec * 1000 + now.tv_usec / 1000);
}

// for avoid the wait of the cpu and run 100% of the cpu
void	pause_execution(long long msec)
{
	long long	start;

	start = get_timestamp();
	while (get_timestamp() - start < msec)
		usleep(500);
}

// Safe function to check if simulation has ended
int	is_simulation_ended(t_table *table)
{
	int	ended;

	pthread_mutex_lock(&table->end_lock);
	ended = table->is_ended;
	pthread_mutex_unlock(&table->end_lock);
	return (ended);
}

// Safe function to set simulation end
void	set_simulation_end(t_table *table)
{
	pthread_mutex_lock(&table->end_lock);
	table->is_ended = 1;
	pthread_mutex_unlock(&table->end_lock);
}
