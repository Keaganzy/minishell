/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 23:15:23 by jotong            #+#    #+#             */
/*   Updated: 2025/10/20 09:55:33 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cleanup_and_wait(int *pipe_fd, int *status, pid_t left_pid, pid_t right_pid)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(right_pid, status, 0);
	waitpid(left_pid, NULL, 0);
	if (WIFEXITED(*status))	// return status of the rightmost command.
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_pipe(t_ast *root, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	left_pid = fork();	// execute left command
	if (left_pid == 0)
	{
		close(pipe_fd[0]);		// close read end
		dup2(pipe_fd[1], STDOUT_FILENO);	// redirect stdout to write end
		close(pipe_fd[1]);
		execute_ast(root->left, shell);
		exit(shell->exit_code);
	}
	right_pid = fork();
	if(right_pid == 0)
	{
		close(pipe_fd[1]); // close write end
		dup2(pipe_fd[0], STDIN_FILENO);	// redirect STDIN from read end
		close(pipe_fd[0]);
		execute_ast(root->right, shell);
		exit(shell->exit_code);
	}
	return (cleanup_and_wait(pipe_fd, &status, left_pid, right_pid));
}

int	execute_cmd(t_ast *root, t_shell *shell)
{
	// apply redirections first
	if (apply_redirections(root) == -1)
		return (1);
	if (is_builtin(root))
		return (execute_builtin(root, shell));
	return (1); // TODO: check if this is correct
}

int	execute_redir(t_ast *root, t_shell *shell)
{
	if (apply_redirection_to_curr_fd(root) == -1)
		return (1);
	return (execute_ast(root->left, shell));
}

int	apply_redirection_to_curr_fd(t_ast *root) // TODO: need to figure out input arg (root / curr node?)
{
	int	file_fd;
	int	target_fd;

	if (!root || !root->filename)
		return(1);	// missing filename
	if (root->type == N_REDIR_OUT)
	{
		file_fd = open(root->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		target_fd = STDOUT_FILENO;
	}
	else if (root->type == N_REDIR_IN)
	{
		file_fd = open(root->filename, O_RDONLY);
		target_fd = STDIN_FILENO;
	}
	else
	{
		// TODO: Handle heredoc, N_REDIR_APPEND here
		return (0);
	}
	if (file_fd == -1) // fileopen error
	{
		perror(root->filename);
		return (1);
	}
	if (dup2(file_fd, target_fd) == -1)
	{
		perror("dup2 failed");
		close(file_fd);
		return (1);
	}
	close(file_fd);
	return (0);
}
