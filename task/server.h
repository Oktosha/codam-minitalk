/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 19:23:01 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 16:11:43 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <signal.h>

# define BITSTRING_FALLBACK_DATA_SIZE 4

// Encoding and decoding data from global variable in server
sig_atomic_t	encode_message(int pid, int signal);
int				is_clear(sig_atomic_t message);
int				is_error(sig_atomic_t message);
int				get_sender(sig_atomic_t message);
int				get_signal(sig_atomic_t message);

// data structure to hold the messages
typedef struct s_bitstring
{
	char	*data;
	char	fallback_data[BITSTRING_FALLBACK_DATA_SIZE];
	int		char_capacity;
	int		bit_length;
}	t_bitstring;

int				bitstring_append_bit(t_bitstring *bitstring, int bit);
t_bitstring		bitstring_create_empty(void);
int				bitstring_is_finished(t_bitstring *bitstring);
void			bitstring_reset(t_bitstring *bitstring);
void			bitstring_soft_reset(t_bitstring *bitstring);
#endif
