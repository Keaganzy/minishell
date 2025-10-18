/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/10/18 23:46:40 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_flag_n(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');	// returns true if "-n", "-nnn" etc
}

int	ft_echo(char **av, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	i = 1;
	n_flag = 0;
	if (av[i] && is_flag_n(av[i]))
	{
		n_flag = 1;
		while (av[i] && is_flag_n(av[i]))
			i++;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		// if (av[i+1])
		// 	printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
