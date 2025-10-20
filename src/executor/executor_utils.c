/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 23:15:23 by jotong            #+#    #+#             */
/*   Updated: 2025/10/21 00:08:20 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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

int	execute_pipe(t_ast *curr, t_shell *shell)
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
		execute_ast(curr->left, shell);
		exit(shell->exit_code);
	}
	right_pid = fork();
	if(right_pid == 0)
	{
		close(pipe_fd[1]); // close write end
		dup2(pipe_fd[0], STDIN_FILENO);	// redirect STDIN from read end
		close(pipe_fd[0]);
		execute_ast(curr->right, shell);
		exit(shell->exit_code);
	}
	return (cleanup_and_wait(pipe_fd, &status, left_pid, right_pid));
}

int	execute_cmd(t_ast *curr, t_shell *shell)
{
	char	*cmd_path;

	cmd_path = NULL;
	// apply redirections first
	if (apply_redirections(curr) == -1)
		return (1);
	if (is_builtin(curr))
		return (execute_builtin(curr, shell));
	// cmd_path = getenv_value(shell->env, "PATH");
	cmd_path = find_full_path(shell->env, curr->argv[0]);
	if (cmd_path == NULL)
	{
		printf("%s: command not found.\n", curr->argv[0]);
		return (127);	// exit code for Command not found.
	}
	execve(cmd_path, curr->argv, shell->env);
	free(cmd_path);
	perror("execve failed");
	return (126);  // exit code for execution failed
}

char	*find_full_path(char **env, char *av)
{
	char	*path;
	int		p_len;

	p_len = 0;
	path = NULL;
	if (!env || !av)
		return (NULL);
	if (av[0] == '\\') // check if the first char of provided file is "\"
		path = ft_strdup(av);
	else
	{
		p_len = ft_strlen(getenv_value(env, "PATH")) + 1;
		path = ft_strndup(getenv_value(env, "PATH"), p_len);
		if (!path)
			return (NULL);
		path = realloc(path, p_len + ft_strlen(av));
		ft_strlcat(path, av, p_len);
	}
	return (path);
}
