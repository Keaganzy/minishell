/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/07 19:38:25 by jotong           ###   ########.fr       */
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

// static int	has_some_invalid_args(int num_args, char **av)
// {
// 	int	i;
// 	int	x;
	
// 	i = 1;
// 	x = 0;
// 	while (i < num_args)
// 	{
// 		x = 0;
// 		while (av[i][x])
// 		{
// 			if (is_digit(av[i][x]))
// 				x++;
// 			else
// 				return (0);
// 		}
// 	}
// 	return (1);
// }

static void	handle_exit_and_cleanup(int exit_code, char **av, t_shell *shell)
{
	printf("exit\n");
	if (av[1])
	{
		exit_code = ft_atoi(av[1]);
		exit_code = exit_code & 255;
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
		if (av[1] && av[1][i] != '\0')
		{
			printf("exit: numeric argument required.\n");
			return (1);
		}
	}
	else if (num_args > 2)
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	return (handle_exit_and_cleanup(exit_code, av, shell), 0);
}

int	ft_pwd(char **av, t_shell *shell)
{
	char	c[PATH_MAX];

	(void)av;
	(void)shell;
	if (getcwd(c, sizeof(c)) != 0)
	{
		printf("%s\n", c);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
