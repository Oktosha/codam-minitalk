/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_message.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 19:23:01 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/20 22:37:01 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MESSAGE_H
# define SERVER_MESSAGE_H

# include <signal.h>

sig_atomic_t	encode_message(int pid, int signal);
int				is_clear(sig_atomic_t message);
int				is_error(sig_atomic_t message);
int				get_sender(sig_atomic_t message);
int				get_signal(sig_atomic_t message);

#endif
