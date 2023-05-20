/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_message.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 19:21:49 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/21 01:13:27 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

#include "server_message.h"

sig_atomic_t	encode_message(int pid, int signal)
{
	return ((pid << 5) + signal);
}

int	is_clear(sig_atomic_t message)
{
	return (message == 0);
}

int	is_error(sig_atomic_t message)
{
	return (message == -1);
}

int	get_sender(sig_atomic_t message)
{
	if (message < 0)
		return (message);
	return (message >> 5);
}

int	get_signal(sig_atomic_t message)
{
	const int	signal_mask = 31;

	if (message < 0)
		return (message);
	return (message & signal_mask);
}
