/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_client_args.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/19 17:29:07 by dkolodze      #+#    #+#                 */
/*   Updated: 2023/05/19 17:36:23 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CLIENT_ARGS_H
# define PARSE_CLIENT_ARGS_H

// parsing args

typedef struct s_client_args
{
	int		server_pid;
	char	*data;
	int		data_bit_length;
}	t_client_args;

t_client_args	failfast_parse_args(int argc, char **argv);

#endif
