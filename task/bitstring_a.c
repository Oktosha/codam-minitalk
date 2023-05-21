/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bitstring_a.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 20:34:18 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/21 02:20:09 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "bitstring.h"

#include "memory.h"
#include "print.h"

static int	bitstring_increase_capacity(t_bitstring *bitstring)
{
	int		i;
	int		new_capacity;
	char	*new_data;
	char	*old_data;

	new_capacity = BITSTRING_FALLBACK_DATA_SIZE * 2;
	if (bitstring->char_capacity * 2 > new_capacity)
		new_capacity = bitstring->char_capacity * 2;
	new_data = malloc_wrapper(sizeof(char) * new_capacity);
	if (!new_data)
		return (0);
	i = -1;
	while (++i < new_capacity)
		new_data[i] = 0;
	if (bitstring->data)
		old_data = bitstring->data;
	else
		old_data = bitstring->fallback_data;
	i = -1;
	while (++i < bitstring->char_capacity)
		new_data[i] = old_data[i];
	free(bitstring->data);
	bitstring->char_capacity = new_capacity;
	bitstring->data = new_data;
	return (1);
}

int	bitstring_append_bit(t_bitstring *bitstring, int bit)
{
	int		pos_in_byte;
	char	last_byte;
	char	*data;

	if (bitstring->char_capacity * 8 < (bitstring->bit_length + 1))
		if (!bitstring_increase_capacity(bitstring))
			return (0);
	if (bitstring->data)
		data = bitstring->data;
	else
		data = bitstring->fallback_data;
	last_byte = data[bitstring->bit_length / 8];
	pos_in_byte = bitstring->bit_length % 8;
	last_byte = last_byte + (bit << pos_in_byte);
	data[bitstring->bit_length / 8] = last_byte;
	bitstring->bit_length += 1;
	return (1);
}

t_bitstring	bitstring_create_empty(void)
{
	t_bitstring	bitstring;

	bitstring.bit_length = 0;
	bitstring.char_capacity = BITSTRING_FALLBACK_DATA_SIZE;
	bitstring.data = NULL;
	bitstring_soft_reset(&bitstring);
	return (bitstring);
}

int	bitstring_is_finished(t_bitstring *bitstring)
{
	char	*data;

	if (bitstring->data)
		data = bitstring->data;
	else
		data = bitstring->fallback_data;
	return ((bitstring->bit_length > 0) && (bitstring->bit_length % 8 == 0) \
		&& (data[(bitstring->bit_length / 8) - 1] == 0));
}

void	bitstring_print(t_bitstring *bitstring)
{
	char	*data;

	if (bitstring->data)
		data = bitstring->data;
	else
		data = bitstring->fallback_data;
	write(STDOUT_FILENO, data, bitstring->bit_length / 8);
	if (bitstring->bit_length % 8)
	{
		print(STDERR_FILENO, "Warning: attempt to print bitstring");
		print(STDERR_FILENO, " holding non-whole amount of bytes\nlast ");
		print(STDERR_FILENO, "%d bits ommitted \n", bitstring->bit_length % 8);
	}
}
