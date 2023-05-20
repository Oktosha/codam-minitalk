/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 22:51:11 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/20 23:59:20 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bitstring.h"
#include "common.h"
#include "print.h"
#include "server_message.h"

volatile sig_atomic_t	g_message = 0;

static void	server_sigusr_handler(int sig, siginfo_t *info, void *uap)
{
	if (!is_error(g_message) \
		&& (is_clear(g_message) \
		|| (get_sender(g_message) == info->si_pid && !get_signal(g_message))))
		g_message = encode_message(info->si_pid, sig);
	else
		g_message = -1;
}

static void	save_and_confirm(int message, t_bitstring *bitstring, int *wait_cnt)
{
	int	answer;
	int	bit;

	answer = SIGUSR1;
	bit = signal_to_bit(get_signal(message));
	if (!bitstring_append_bit(bitstring,  bit))
	{
		answer = SIGUSR2;
		print(STDERR_FILENO, "Warning: can't allocate memory\n");
		bitstring_print(bitstring);
		bitstring_soft_reset(bitstring);
		bitstring_append_bit(bitstring, bit);
	}
	g_message = encode_message(get_sender(message), 0);
	*wait_cnt = 0;
	kill(get_sender(message), answer);
}

static void	print_and_reset(int *message, t_bitstring *bitstring, int *wait_cnt)
{
	bitstring_print(bitstring);
	print(STDOUT_FILENO, "\n");
	bitstring_reset(bitstring);
	g_message = 0;
	*wait_cnt = 0;
	message = 0;
}

int main()
{
	int			wait_cnt;
	int			message;
	t_bitstring	bitstring;

	print(STDOUT_FILENO, "%d\n", getpid());
	add_sigusr_handler(server_sigusr_handler);
	wait_cnt = 0;
	bitstring = bitstring_create_empty();
	message = g_message;
	while(!is_error(message))
	{
		if (get_signal(message))
			save_and_confirm(message, &bitstring, &wait_cnt);
		if (bitstring_is_finished(&bitstring))
			print_and_reset(&message, &bitstring, &wait_cnt);
		if (get_sender(message))
			wait_cnt += 1;
		if (wait_cnt > 1000)
		{
			print(STDERR_FILENO, "Warning: timeout for %d\n", get_sender(message));
			print_and_reset(&message, &bitstring, &wait_cnt);
		}
		usleep(50);
		message = g_message;
	}
	print(STDERR_FILENO, "Error: received mixed signals\n");
	exit(EXIT_MIXED_SIGNALS);
}
