/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 22:51:11 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 15:48:45 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t	G_signal = 0;
volatile sig_atomic_t	G_pid = 0;
volatile sig_atomic_t	received_sigs = 0;
volatile sig_atomic_t	handled_sigs = 0;

void handler(int sig, siginfo_t *info, void *uap)
{
	received_sigs += 1;
	if ((!G_signal) && (!G_pid || G_pid == info->si_pid))
	{
		handled_sigs += 1;
		G_signal = sig;
		G_pid = info->si_pid;
	}
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

int main()
{
	printf("%d\n", getpid());
	add_handler(SIGUSR1, SIGUSR2);
	add_handler(SIGUSR2, SIGUSR1);
	int sent_back = 0;
	int wait_cnt = 0;
	int cnt = 0;
	while(cnt < 1000000000) {
		if (G_signal)
		{
			sent_back += 1;
			int tmp_signal = G_signal;
			G_signal = 0;
			kill(G_pid, tmp_signal);
			wait_cnt = 0;
		}
		else if (G_pid)
		{
			wait_cnt += 1;
		}
		usleep(50);
		if (wait_cnt > 1000 && G_pid)
		{
			printf("disconnecting from client %d\n", G_pid);
			G_pid = 0;
			wait_cnt = 0;
		}
		cnt += 1;
		if (cnt % 10000 == 0)
			printf("pid: %d signal: %d wait: %d\n", G_pid, G_signal, wait_cnt);

	}
}
