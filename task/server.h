/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 19:23:01 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 19:27:24 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <signal.h>

// Encoding and decoding data from global variable in server
sig_atomic_t	encode_message(int pid, int signal);
int				is_clear(sig_atomic_t message);
int				is_error(sig_atomic_t message);
int				get_sender(sig_atomic_t message);
int				get_signal(sig_atomic_t message);

#endif
