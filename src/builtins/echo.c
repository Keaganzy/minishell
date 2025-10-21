/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/10/22 00:06:15 by jotong           ###   ########.fr       */
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

static void skip_past_n(char **av, int *n_flag, int *i)
{
	if (av[*i] && is_flag_n(av[*i]))
	{
		*n_flag = 1;
		while (av[*i] && is_flag_n(av[*i]))
			(*i)++;
	}
}

int	ft_echo(char **av, t_shell *shell)
{
	int		i;
	int		n_flag;
	char	*val;

	i = 1;
	n_flag = 0;
	skip_past_n(av, &n_flag, &i);
	while (av[i])
	{
		if (i > 1 && !is_flag_n(av[i-1]))
			printf(" ");
		if (av[i][0] == '$')
		{
			val = getenv_value(shell->env, &av[i][1]);
			if (val != NULL)
				printf("%s", val);
		}
		else
			printf("%s", av[i]);
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
