/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   common.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/19 17:08:43 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 18:49:49 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

#include "common.h"

void add_sigusr_handler(void *handler)
{
	struct sigaction	action;

	action.__sigaction_u.__sa_sigaction = handler;
	sigemptyset(&(action.sa_mask));
	sigaddset(&(action.sa_mask), SIGUSR1);
	sigaddset(&(action.sa_mask), SIGUSR2);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
}

int	bit_to_signal(int bit)
{
	if (bit == 0)
		return (SIGUSR1);
	if (bit == 1)
		return (SIGUSR2);
	exit(3);
}

int	signal_to_bit(int signal)
{
	if (signal == SIGUSR1)
		return (0);
	if (signal == SIGUSR2)
		return (1);
	exit(4);
}
