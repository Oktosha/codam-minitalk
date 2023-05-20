/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bitstring_b.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/20 23:41:15 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/21 00:29:27 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "bitstring.h"

void	bitstring_reset(t_bitstring *bitstring)
{
	bitstring_soft_reset(bitstring);
	free(bitstring->data);
	bitstring->data = NULL;
	bitstring->char_capacity = BITSTRING_FALLBACK_DATA_SIZE;
}

void	bitstring_soft_reset(t_bitstring *bitstring)
{
	int	i;

	if (bitstring->data)
	{
		i = 0;
		while (i < bitstring->char_capacity)
		{
			bitstring->data[i] = 0;
			i += 1;
		}
	}
	i = 0;
	while (i < BITSTRING_FALLBACK_DATA_SIZE)
	{
		bitstring->fallback_data[i] = 0;
		i += 1;
	}
	bitstring->bit_length = 0;
}
