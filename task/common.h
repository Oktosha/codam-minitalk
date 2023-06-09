/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   common.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/19 16:48:02 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/20 22:17:00 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include <signal.h>

# define SIGNAL_SERVER_CONFIRM SIGUSR1
# define SIGNAL_SERVER_WARNING SIGUSR2

# define EXIT_LOST_INTEGRITY -1
# define EXIT_TIMEOUT 1
# define EXIT_MIXED_SIGNALS 2
# define EXIT_WRONG_ARGS 3

void	add_sigusr_handler(void *handler);
int		bit_to_signal(int bit);
int		signal_to_bit(int signal);

#endif
