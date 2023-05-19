/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 22:51:11 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 16:05:51 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "server.h"

volatile sig_atomic_t	g_message = 0;

static void	handler(int sig, siginfo_t *info, void *uap)
{
	if (!is_error(g_message) \
		&& (is_clear(g_message) \
		|| (get_sender(g_message) == info->si_pid && !get_signal(g_message))))
		g_message = encode_message(info->si_pid, sig);
	else
		g_message = -1;
}

static void add_handlers(void)
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

int main()
{
	int			wait_cnt;
	int			message;
	t_bitstring	bitstring;

	printf("%d\n", getpid());
	add_handlers();
	wait_cnt = 0;
	bitstring = bitstring_create_empty();
	message = g_message;
	int mem_status = 1;
	while(!is_error(message)) {
		if (get_signal(message))
		{
			int answer = SIGUSR1;
			mem_status = bitstring_append_bit(&bitstring, get_signal(message) % 2);
			if (!mem_status)
			{
				answer = SIGUSR2;
				printf("couldn't allocate more mem for the message\n");
				printf("message is %d bits so far\n", bitstring.bit_length);
				printf("writing so far received bytes to the stdout\n");
				// TODO : print partial message
				printf("storing the new bytes over the old bytes\n");
				bitstring_soft_reset(&bitstring);
				bitstring_append_bit(&bitstring, get_signal(message) % 2);
			}
			g_message = encode_message(get_sender(message), 0);
			wait_cnt = 0;
			kill(get_sender(message), answer);
		}
		if (bitstring_is_finished(&bitstring))
		{
			printf("%s\n", bitstring.data);
			bitstring_reset(&bitstring);
			g_message = 0;
			wait_cnt = 0;
		}
		if (get_sender(message))
			wait_cnt += 1;
		if (wait_cnt > 1000)
		{
			printf("timeout for %d\n", get_sender(message));
			bitstring_reset(&bitstring);
			g_message = 0;
			wait_cnt = 0;
		}
		usleep(50);
		message = g_message;
	}
	printf("Error occured: either a client doesn't follow the protocol or 2 clients tried to connect\n");
}
