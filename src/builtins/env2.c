/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:44:16 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 08:50:55 by codespace        ###   ########.fr       */
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
	int	result;

	i = 1;
	result = 0;
	while (av[i])
	{
		if (ft_strchr(av[i], '=') != NULL)
		{
			if (add_update_env_vars(shell, av[i]) != 0)
				*status = 1;
		}
		else
		{
			result = validate_key(av[i]);
			if (result != 0)
				*status = 1;
		}
		i++;
	}
	return ;
}

int	ft_export(char **av, t_shell *shell)
{
	int	status;

	status = 0;
	if (!shell || !shell->envp)
		return (1);
	if (av[1])
	{
		extract_and_update_vars(av, shell, &status);
		return (status);
	}
	return (status);
}

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

int	validate_identifiers(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (av[i])
	{
		j = 1;
		if (!(ft_isalpha(av[i][0]) || av[i][0] == '-'))
			return (0);
		while (av[i][j])
		{
			if (!ft_isalnum(av[i][j]) || av[i][j] == '-')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
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
