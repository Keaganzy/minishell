/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/18 15:25:26 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

int	ft_env(char **av, t_shell *shell)
{
	int	i;

	(void)av;
	if (!shell || !shell->envp)
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}

int	ft_export(char **av, t_shell *shell)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	if (!shell || !shell->envp)
		return (1);
	if (av[1])
	{
		if (ft_strchr(av[1], '=') != NULL)
			while (av[i])
			{
				if (add_update_env_vars(shell, av[i]) != 0)
					status = 1;
				i++;
			}
		return (status);
	}
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (status);
}

int	ft_unset(char **av, t_shell *shell)
{
	int	i;

	i = 1;
	if (!av)
		return (1);
	if (!av[1])
	{
		printf("unset: not enough arguments.\n");
		return (1);
	}
	while (av[i])
	{
		if (getenv_value(shell->envp, av[i]) == NULL)
			continue ;
		else
			unsetenv_value(&shell->envp, av[i]);
		i++;
	}

	return (0);
}

int	ft_exit(char **av, t_shell *shell)
{
	(void)av;
	(void)shell;

	printf("exit\n");
	cleanup_shell(shell);
	exit(0);
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
