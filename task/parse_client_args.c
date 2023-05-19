/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_client_args.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/19 17:48:42 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 19:00:19 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "parse_client_args.h"

#include "print.h"
#include "signed_strlen.h"

# define USAGE_EXAMPLE "usage: ./client server_pid message\n"

static int	is_decimal_digits(char *s)
{
	while (*s)
	{
		if ((*s < '0') || (*s > '9'))
		{
			return (0);
		}
		++s;
	}
	return (1);
}

static int	is_bigger_than_intmax(char *s)
{
	const char	*int_max_s = "2147483647";
	int			i;
	int			len;

	while (*s == '0')
		++s;
	len = signed_strlen(s);
	if (len != signed_strlen(int_max_s))
		return (len > signed_strlen(int_max_s));
	i = 0;
	while (i < len)
	{
		if (s[i] != int_max_s[i])
			return (s[i] > int_max_s[i]);
		i += 1;
	}
	return (0);
}

static int extract_int_from_clean_string(char *s)
{
	int	ans;

	ans = 0;
	while (*s)
	{
		ans = ans * 10 + (*s - '0');
		s++;
	}
	return (ans);
}

static int	failfast_parse_pid(char *s)
{
	if (signed_strlen(s) == 0)
	{
		print(STDERR_FILENO, "pid arg is empty string\n%s", USAGE_EXAMPLE);
		exit(7);
	}
	if (!is_decimal_digits(s))
	{
		print(STDERR_FILENO, "pid arg '%s' contains non-digits; ", s);
		print(STDERR_FILENO, "'+' and whitespace arn't allowed, too\n");
		print(STDERR_FILENO, USAGE_EXAMPLE);
		exit(8);
	}
	if ((s[0] == '0') && (s[1] != '\0'))
		print(STDERR_FILENO, "Leading zeroes? Ignoring them judgementally\n");
	if (is_bigger_than_intmax(s))
	{
		print(STDERR_FILENO, "pid = %s > INT_MAX: it's not a valid pid\n");
		print(STDERR_FILENO, USAGE_EXAMPLE);
		exit(9);
	}
	return (extract_int_from_clean_string(s));
}

t_client_args	failfast_parse_args(int argc, char **argv)
{
	t_client_args	args;

	if (argc != 3)
	{
		print(STDERR_FILENO, "argc = %d\n");
		print(STDERR_FILENO, "argc should be = 3\n");
		print(STDERR_FILENO, USAGE_EXAMPLE);
		exit(6);
	}
	args.data = argv[2];
	args.data_bit_length = 8 * (signed_strlen(argv[2]) + 1);
	args.server_pid = failfast_parse_pid(argv[1]);
	return (args);
}
