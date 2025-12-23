/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/12/23 18:05:20 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

static void	extract_and_update_vars(char **av, t_shell *shell, int *status)
{
	int	i;

	i = 1;
	if (ft_strchr(av[1], '=') != NULL)
	{
		while (av[i])
		{
			if (add_update_env_vars(shell, av[i]) != 0)
				*status = 1;
			i++;
		}
	}
	return ;
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
		extract_and_update_vars(av, shell, &status);
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
	while (av[i])
	{
		if (validate_identifiers(av) != 1)
		{
			printf("Invalid identifier\n");
			return (1);
		}
		printf("i in validate : %d\n", i);
		i++;
		
	}
	i = 1;
	while (av[i])
	{
		if (getenv_value(shell->envp, av[i]) != NULL)
			unsetenv_value(&shell->envp, av[i]);
		printf("i in unset : %d\n", i);
		i++;
		
	}
	return (0);
}

int	ft_exit(char **av, t_shell *shell)
{
	int	i;

	(void)shell;
	i = 0;
	while (av[1][i] && ((av[1][i] >= '0' && av[1][i] <= '9') || 
            (i == 0 && (av[1][i] == '+' || av[1][i] == '-'))))
		i++;
	if (av[1][i] != '\0')
	{
		printf("exit: numeric argument required.\n");
		return (1);
	}
	printf("exit\n");
	cleanup_shell(shell);
	if (av[1])
		exit(ft_atoi(av[1]));
	else
		exit(0);
}

int	ft_pwd(char **av, t_shell *shell)
{
	char	c[PATH_MAX];

	(void)av;
	(void)shell;
	if (getcwd(c, sizeof(c)) != 0)
	{
		printf("%s\n", c); // dont remove this, this is intended
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
