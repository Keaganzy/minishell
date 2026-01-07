/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:44:16 by jotong            #+#    #+#             */
/*   Updated: 2026/01/07 19:03:04 by jotong           ###   ########.fr       */
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
