/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 21:06:37 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 21:26:28 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t waiting = 0;

void handler(int signal, siginfo_t *info, void *uap)
{
	if (waiting == info->si_pid)
		waiting = 0;
	else
		waiting = -1;
}

int get_bit(const char *data, int bit_pos)
{
	char	byte;
	int		bit;

	byte = data[bit_pos / 8];
	bit = (1 << (bit_pos % 8)) & byte;
	bit = bit >> (bit_pos % 8);
	return (bit);
}

int main(int argc, char**argv)
{
	if (argc != 3)
	{
		printf("Wrong amount of args: %d; expected 2: server PID and message\n", argc);
		exit(1);
	}

	struct sigaction action;
	action.__sigaction_u.__sa_sigaction = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);

	int pid = atoi(argv[1]);
	int pos = 0;
	int len = strlen(argv[2]);
	while (pos < (len + 1) * 8)
	{
		int bit = get_bit(argv[2], pos);
		int signal = SIGUSR1;
		if (signal % 2 != bit % 2)
			signal = SIGUSR2;
		pos += 1;
		waiting = pid;
		kill(pid, signal);
		int wait_count = 0;
		while(waiting && wait_count < 1000)
		{
			usleep(10);
			wait_count += 1;
		}
		if (waiting)
		{
			printf("No confirmation from server %d\n", waiting);
			break;
		}
	}
}
