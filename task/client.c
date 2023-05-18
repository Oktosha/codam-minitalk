/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 21:41:57 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 00:23:49 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_CNT 1000

volatile sig_atomic_t waiting = 0;
int pid = -1;

void handler(int signal, siginfo_t *info, void *uap)
{
	waiting = 0;
}

int main(int argc, char**argv)
{
	int cnt = 0;
	if (argc < 2)
	{
		printf("too few args, need server id\n");
		exit(1);
	}
	else if (argc == 2)
	{
		pid = atoi(argv[1]);
		cnt = DEFAULT_CNT;
	}
	else if (argc == 3)
	{
		pid = atoi(argv[1]);
		cnt = atoi(argv[2]);
	}
	else
	{
		printf("too many args");
		exit(1);
	}
	struct sigaction action;
	action.__sigaction_u.__sa_sigaction = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);

	struct timespec start;
	clock_gettime(CLOCK_REALTIME, &start);
	int sent = 0;
	int missed = 0;
	while(sent < cnt)
	{
		waiting = 1;
		kill(pid, SIGUSR1);
		int wait_count = 0;
		while(waiting && wait_count < 1000)
		{
			usleep(10);
			wait_count += 1;
		}
		if (waiting) {
			missed += 1;
		}
		sent += 1;
	}
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	double elapsed = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
	printf("elapsed for %d: %.3f\n", sent, elapsed / 1000000000);
	printf("missed: %d\n", missed);
}
