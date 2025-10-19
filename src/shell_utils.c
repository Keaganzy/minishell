/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/10/19 23:57:59 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_shell(t_shell *shell)
{
	// int	i;

	// i = 0;
	// while (shell->env[i])
	// {
	// 	free(shell->env[i]);
	// 	i++;
	// }
	// free(shell->env);
	(void)shell;
	// token_free_all(&tokens);
	return ;
}
