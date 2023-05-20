/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bitstring.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/20 22:22:53 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/21 00:24:53 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITSTRING_H
# define BITSTRING_H

# define BITSTRING_FALLBACK_DATA_SIZE 4

typedef struct s_bitstring
{
	char	*data;
	char	fallback_data[BITSTRING_FALLBACK_DATA_SIZE];
	int		char_capacity;
	int		bit_length;
}	t_bitstring;

// bitstring_a.c
int			bitstring_append_bit(t_bitstring *bitstring, int bit);
t_bitstring	bitstring_create_empty(void);
int			bitstring_is_finished(t_bitstring *bitstring);
void		bitstring_print(t_bitstring *bitstring);

// bitstring_b.c
void		bitstring_reset(t_bitstring *bitstring);
void		bitstring_soft_reset(t_bitstring *bitstring);

#endif
