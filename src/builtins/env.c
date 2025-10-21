/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/10/21 23:06:27 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#if defined(__LINUX___)
	#include <linux/limits.h>
#endif

int	ft_env(char **av, t_shell *shell)
{
	int	i;

	(void)av;
	if (!shell || !shell->env)
		return (1);
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
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
	if (!shell || !shell->env)
		return (1);
	if (av[1])
	{
		while (av[i])
		{
			if (add_update_env_vars(shell, av[i]) != 0)
				status = 1;
			i++;
		}
		return (status);
	}
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	return (status);
}

int	ft_unset(char **av, t_shell *shell)
{
	(void)av;
	(void)shell;
	// nothing to unset cos no options
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
