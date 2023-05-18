/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 22:51:11 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 21:05:32 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "server.h"

volatile sig_atomic_t	g_message = 0;

void	handler(int sig, siginfo_t *info, void *uap)
{
	if (!is_error(g_message) \
		&& (is_clear(g_message) \
		|| (get_sender(g_message) == info->si_pid && !get_signal(g_message))))
		g_message = encode_message(info->si_pid, sig);
}

void add_handler(int sig_to_set, int sig_to_ignore)
{
	struct sigaction action;
	action.__sigaction_u.__sa_sigaction = handler;
	sigemptyset(&(action.sa_mask));
	sigaddset(&(action.sa_mask), sig_to_ignore);
	action.sa_flags = SA_SIGINFO;
	sigaction(sig_to_set, &action, NULL);
}

int is_finished(int pos)
{
	return pos >= 10000;
}

int main()
{
	printf("%d\n", getpid());
	add_handler(SIGUSR1, SIGUSR2);
	add_handler(SIGUSR2, SIGUSR1);
	int sent_back = 0;
	int wait_cnt = 0;
	t_bitstring bitstring = bitstring_create_empty();
	while(1) {
		usleep(50);
		int message = g_message;
		int client_pid = get_sender(message);
		int signal = get_signal(message);
		if (is_error(message))
		{
			printf("message is error\n");
			break;
		}
		if (get_signal(message))
		{
			bitstring_append_bit(&bitstring, signal % 2);
			g_message = encode_message(client_pid, 0);
			wait_cnt = 0;
			kill(client_pid, SIGUSR1);
		}
		if (bitstring_is_finished(&bitstring))
		{
			printf("%s\n", bitstring.data);
			g_message = 0;
			wait_cnt = 0;
			bitstring_reset(&bitstring);
		}
		if (get_sender(message))
			wait_cnt += 1;
		if (wait_cnt > 1000)
		{
			int client_pid = get_sender(message);
			printf("disconnecting from %d\n", client_pid);
			bitstring_reset(&bitstring);
			g_message = 0;
			wait_cnt = 0;
		}
	}
}
