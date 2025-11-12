/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:56 by jotong            #+#    #+#             */
/*   Updated: 2025/11/12 17:26:11 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **cleanup_dup_envp(t_shell *shell, int index)
{
	int		i;
	char	**duped_env;

	i = 0;
	duped_env = shell->env;
	while (i < index)
	{
		free(duped_env[i]);
		i++;
	}
	free(duped_env);
	shell->env = NULL;
	return (shell->env);
}

// void cleanup_shell(t_shell *shell)
// {
// 	int	i;

// 	i = 0;
// 	while (shell->env[i])
// 		i++;
// 	cleanup_dup_envp(shell, i - 1);
// 	return ;
// }
