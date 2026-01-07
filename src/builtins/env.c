/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/07 20:14:07 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

int	ft_unset(char **av, t_shell *shell)
{
	int	i;

	i = 1;
	if (!av)
		return (1);
	while (av[i])
	{
		if (validate_identifiers(av) != 1)
		{
			printf("Invalid identifier\n");
			return (1);
		}
		i++;
	}
	i = 1;
	while (av[i])
	{
		if (getenv_value(shell->envp, av[i]) != NULL)
			unsetenv_value(&shell->envp, av[i]);
		i++;
	}
	return (0);
}

static int	count_args(char **av)
{
	int	i;

	i = 1;
	while (av[i] && av[i][0] != '\0')
		i++;
	return (i);
}

static int	has_no_invalid_digits(char **av)
{
	int	x;

	x = 0;
	while (av[1][x])
	{
		if (ft_isdigit(av[1][x]))
			x++;
		else
			return (0);
	}
	return (1);
}

static void	handle_exit_and_cleanup(int exit_code, char **av, t_shell *shell)
{
	if (exit_code == 0)
		printf("exit\n");
	if (av[1])
	{
		if (has_no_invalid_digits(av))
		{
			exit_code = ft_atoi(av[1]);
			exit_code = exit_code & 255;
		}
		else
			exit_code = 2;
	}
	else
		exit_code = 0;
	shell->exit_code = exit_code;
	cleanup_shell(shell);
	exit(exit_code);
	return ;
}

int	ft_exit(char **av, t_shell *shell)
{
	int	i;
	int	num_args;
	int	exit_code;

	exit_code = shell->last_exit_status;
	num_args = count_args(av);
	i = 0;
	if (num_args == 2)
	{
		while (av[1] && av[1][i] && ((av[1][i] >= '0' && av[1][i] <= '9')
			|| (i == 0 && (av[1][i] == '+' || av[1][i] == '-'))))
			i++;
		if (av[1][i] != '\0')
			printf("exit: numeric argument required.\n");
		exit_code = 2;
	}
	else if (num_args > 2)
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	return (handle_exit_and_cleanup(exit_code, av, shell), 0);
}
