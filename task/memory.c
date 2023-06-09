/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/20 22:32:19 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/21 15:18:07 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "memory.h"

#ifndef MALLOC_FAILURE_CNT
# define MALLOC_FAILURE_CNT 0
#endif

void	*malloc_wrapper(int size)
{
	static int	cnt = 0;

	cnt += 1;
	if (MALLOC_FAILURE_CNT && (cnt % MALLOC_FAILURE_CNT == 0))
		return (NULL);
	return (malloc(size));
}
