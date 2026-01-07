/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:58:39 by ksng              #+#    #+#             */
/*   Updated: 2026/01/07 14:25:41 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	execute_logical(t_ast *node, t_shell *shell)
{
	int	left_status;
	int	right_status;

	left_status = execute_node(node->left, shell);
	shell->last_exit_status = left_status;
	if (node->type == N_AND)
	{
		if (left_status == 0)
		{
			right_status = execute_node(node->right, shell);
			shell->last_exit_status = right_status;
			return (right_status);
		}
		return (left_status);
	}
	if (node->type == N_OR)
	{
		if (left_status != 0)
		{
			right_status = execute_node(node->right, shell);
			shell->last_exit_status = right_status;
			return (right_status);
		}
		return (left_status);
	}
	return (1);
}
