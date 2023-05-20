/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bitstring_print.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/20 23:41:15 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/20 23:58:08 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "bitstring.h"

#include "print.h"

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
