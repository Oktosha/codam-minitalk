/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/07 19:45:41 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/04/21 18:55:07 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "print.h"

#include "signed_strlen.h"

static void	print_string(int file_descriptor, const char *s)
{
	write(file_descriptor, s, signed_strlen(s));
}

static void	print_char(int file_descriptor, char ch)
{
	write(file_descriptor, &ch, 1);
}

static void	print_number(int file_descriptor, int x)
{
	const char	*digits = "0123456789";

	if (x == INT_MIN)
	{
		print_string(file_descriptor, "-2147483648");
		return ;
	}
	if (x < 0)
	{
		write(file_descriptor, "-", 1);
		x = -x;
	}
	if (x > 9)
		print_number(file_descriptor, x / 10);
	write(file_descriptor, digits + (x % 10), 1);
}

void	print(int file_descriptor, const char *format, ...)
{
	va_list	data;

	va_start(data, format);
	while (*format)
	{
		if (*format != '%')
		{
			print_char(file_descriptor, *format);
			++format;
			continue ;
		}
		if (format[1] == 'd')
			print_number(file_descriptor, va_arg(data, int));
		else if (format[1] == 's')
			print_string(file_descriptor, va_arg(data, char *));
		else
		{
			write(STDERR_FILENO, "invalid format: ", 16);
			write(STDERR_FILENO, format, signed_strlen(format));
			exit(1);
		}
		format += 2;
	}
	va_end(data);
}
