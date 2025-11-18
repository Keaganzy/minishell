/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:58:39 by ksng              #+#    #+#             */
/*   Updated: 2025/11/18 21:59:48 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	execute_logical(t_ast *node, t_shell *shell)
{
	int	left_status;

	left_status = execute_node(node->left, shell);
	shell->last_exit_status = left_status;
	if (node->type == N_AND)
	{
		if (left_status == 0)
			return (execute_node(node->right, shell));
		return (left_status);
	}
	if (node->type == N_OR)
	{
		if (left_status != 0)
			return (execute_node(node->right, shell));
		return (left_status);
	}
	return (1);
}
