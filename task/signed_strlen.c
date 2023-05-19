/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signed_strlen.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/17 00:32:58 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/04/21 14:20:33 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "signed_strlen.h"

int	signed_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
	{
		++len;
	}
	return (len);
}
