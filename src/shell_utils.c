/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/10/15 14:54:20 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_shell(t_shell *shell, t_token *tokens)
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
	token_free_all(&tokens);
	return ;
}
