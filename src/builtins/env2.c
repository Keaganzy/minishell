/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:44:16 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 22:05:24 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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
