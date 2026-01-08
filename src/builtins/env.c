/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/08 16:36:35 by codespace        ###   ########.fr       */
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

// static int	has_no_invalid_digits(char *s)
// {
// 	int	x;

// 	x = 0;
// 	while (s[x])
// 	{
// 		if (ft_isdigit(s[x]))
// 			x++;
// 		else
// 			return (0);
// 	}
// 	return (1);
// }

static void	handle_exit_and_cleanup(int exit_code, t_shell *shell)
{
	shell->exit_code = exit_code;
	cleanup_shell(shell);
	exit(exit_code);
}

static int	check_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	print_numeric_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	handle_exit_and_cleanup(2, shell);
}

int	ft_exit(char **av, t_shell *shell)
{
	int	num_args;
	int	exit_code;

	num_args = count_args(av);
	ft_putstr_fd("exit\n", 1);
	if (num_args >= 2)
	{
		if (!check_numeric_arg(av[1]))
			print_numeric_error(av[1], shell);
		if (num_args > 2)
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		exit_code = (unsigned char)ft_atoi(av[1]);
	}
	else
		exit_code = shell->last_exit_status;
	handle_exit_and_cleanup(exit_code, shell);
	return (0);
}
