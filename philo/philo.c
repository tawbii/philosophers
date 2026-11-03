/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atawbi <atawbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 17:50:55 by atawbi            #+#    #+#             */
/*   Updated: 2025/06/29 18:07:02 by atawbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// the main :) no need to explain
int	main(int ac, char **av)
{
	t_table	tb;

	if (ac != 5 && ac != 6)
		return (printf("%s input erorr", av[0]), 1);
	if (string_to_int(av[1]) == 1)
	{
		printf("0 1 is thinking\n");
		usleep(string_to_int(av[2]) * 1000);
		printf("%d 1 has died\n", string_to_int(av[2]));
		return (0);
	}
	setup_dining_table(&tb, av);
	setup_dining_table2(&tb);
	start_simulation(&tb);
	monitor_eaters(&tb);
	finish_simulation(&tb);
	cleanup_dining_table(&tb);
	return (0);
}
