/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 21:06:37 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 17:56:29 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "parse_client_args.h"
#include "print.h"

#define STATUS_GOT_CONFIRMATION 0
#define STATUS_SERVER_WARNING -1
#define STATUS_MIXED_SIGNALS -42

volatile sig_atomic_t	g_status = 0;

static void	client_sigusr_handler(int signal, siginfo_t *info, void *uap)
{
	(void) uap;
	if (g_status == info->si_pid)
	{
		if (signal == SIGNAL_SERVER_CONFIRM)
		{
			g_status = STATUS_GOT_CONFIRMATION;
		}
		else if (signal == SIGNAL_SERVER_WARNING)
		{
			g_status = STATUS_SERVER_WARNING;
		}
	}
	else
	{
		g_status = STATUS_MIXED_SIGNALS;
	}
}

static int	get_bit_at(const char *data, int bit_pos)
{
	char	byte;
	int		bit;

	byte = data[bit_pos / 8];
	bit = (1 << (bit_pos % 8)) & byte;
	bit = bit >> (bit_pos % 8);
	return (bit);
}

static void	handle_possible_error(int status)
{
	if (status > 0)
	{
		print(STDERR_FILENO, "Timeout on waiting for confirmation\n");
		print(STDERR_FILENO, "server with pid = %d haven't sent any\n", status);
		print(STDERR_FILENO, "Is server even running? Is the pid correct?\n");
		exit(1);
	}
	if (status == STATUS_SERVER_WARNING)
	{
		print(STDERR_FILENO, "Server warns: message doesn't fit memory\n");
		print(STDERR_FILENO, "Ignoring the warning and continuing\n");
		g_status = 0;
	}
	if (status == STATUS_MIXED_SIGNALS)
	{
		print(STDERR_FILENO, "Got unexpected signal\n");
		print(STDERR_FILENO, "Either server confirming twice\n");
		print(STDERR_FILENO, "Or non-server process interfering\n");
		exit(2);
	}
	if (status < 0)
	{
		print(STDERR_FILENO, "Unknown status; something is really wrong\n");
		exit(5);
	}
}

int	main(int argc, char**argv)
{
	t_client_args	args;
	int				bit_pos;
	int				wait_count;
	int				signal;

	args = failfast_parse_args(argc, argv);
	add_sigusr_handler(client_sigusr_handler);
	bit_pos = 0;
	while (bit_pos < args.data_bit_length)
	{
		signal = bit_to_signal(get_bit_at(args.data, bit_pos));
		bit_pos += 1;
		g_status = args.server_pid;
		kill(args.server_pid, signal);
		wait_count = 0;
		while (g_status > 0 && wait_count < 1000)
		{
			usleep(10);
			wait_count += 1;
		}
		handle_possible_error(g_status);
	}
}
