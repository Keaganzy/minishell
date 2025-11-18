/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:56:01 by ksng              #+#    #+#             */
/*   Updated: 2025/11/18 22:36:39 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	backup_fds(t_shell *shell)
{
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

static void restore_fds(t_shell *shell)
{
	if (shell->stdin_backup != -1)
	{
		dup2(shell->stdin_backup, STDIN_FILENO);
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup != -1)
	{
		dup2(shell->stdout_backup, STDOUT_FILENO);
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
}

static int	setup_redirections(t_ast *node, t_shell *shell)
{
	if (node->type == N_REDIR_IN)
		return (setup_redir_in(node->filename));
	if (node->type == N_REDIR_OUT)
		return (setup_redir_out(node->filename));
	if (node->type == N_REDIR_APPEND)
		return (setup_redir_append(node->filename));
	if (node->type == N_HEREDOC)
		return (setup_heredoc(node->filename, shell));
	return (1);
}

int	execute_redir(t_ast *node, t_shell *shell)
{
	int status;

	backup_fds(shell);
	status = setup_redirections(node, shell);
	if (status != 0)
	{
		restore_fds(shell);
		return (status);
	}
	status = execute_node(node->left, shell);
	restore_fds(shell);
	return (status);
}
