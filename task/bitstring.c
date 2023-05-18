/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bitstring.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 20:34:18 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/18 21:27:55 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

#include "server.h"

int	bitstring_is_finished(t_bitstring *bitstring)
{
	return ((bitstring->bit_length > 0) && (bitstring->bit_length % 8 == 0) \
		&& (bitstring->data[(bitstring->bit_length / 8) - 1] == 0));
}

static int bitstring_increase_capacity(t_bitstring *bitstring)
{
	int		i;
	int		new_capacity;
	char	*new_data;

	if (bitstring->char_capacity * 8 != bitstring->bit_length)
		exit(2);
	new_capacity = 4;
	if (bitstring->char_capacity * 2 > new_capacity)
		new_capacity = bitstring->char_capacity * 2;
	new_data = malloc(sizeof(char) * new_capacity);
	if (!new_data)
		return (0);
	i = -1;
	while (++i < new_capacity)
		new_data[i] = 0;
	if (bitstring->data)
	{
		i = -1;
		while (++i < bitstring->char_capacity)
			new_data[i] = bitstring->data[i];
		free(bitstring->data);
	}
	bitstring->char_capacity = new_capacity;
	bitstring->data = new_data;
	return (1);
}

int	bitstring_append_bit(t_bitstring *bitstring, int bit)
{
	int		pos_in_byte;
	char	last_byte;

	if (bitstring->char_capacity * 8 < (bitstring->bit_length + 1))
		if (!bitstring_increase_capacity(bitstring))
			return (0);
	last_byte = bitstring->data[bitstring->bit_length / 8];
	pos_in_byte = bitstring->bit_length % 8;
	last_byte = last_byte + (bit << pos_in_byte);
	bitstring->data[bitstring->bit_length / 8] = last_byte;
	bitstring->bit_length += 1;
	return (1);
}

t_bitstring	bitstring_create_empty(void)
{
	t_bitstring	bitstring;

	bitstring.bit_length = 0;
	bitstring.char_capacity = 0;
	bitstring.data = NULL;
	return(bitstring);
}

void	bitstring_reset(t_bitstring *bitstring)
{
	free(bitstring->data);
	bitstring->data = NULL;
	bitstring->bit_length = 0;
	bitstring->char_capacity = 0;
}
