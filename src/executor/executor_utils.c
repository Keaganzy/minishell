/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 23:15:23 by jotong            #+#    #+#             */
/*   Updated: 2025/12/09 18:18:34 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	**get_paths_from_env(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			return (ft_split(path_value, ':'));
		}
		i++;
	}
	return (NULL);
}

int	setup_redir_in(char *filename, t_shell *shell)
{
	int fd;
	
	//jtfunction(filename);//
	filename = expand_and_replace(&filename, shell);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	setup_redir_out(char *filename, t_shell *shell)
{
	int fd;

	//jtfunction(filename);//
	filename = expand_and_replace(&filename, shell);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	dup2(fd,STDOUT_FILENO);
	close(fd);
	return(0);
}

int	setup_redir_append(char *filename, t_shell *shell)
{
	int fd;

	//jtfunction(filename);//
	filename = expand_and_replace(&filename, shell);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	setup_heredoc(t_ast *node, t_shell *shell)
{
	int		pipefd[2];

	(void)shell;
	if (!node->heredoc_content)
		return (1);

	if (pipe(pipefd) == -1)
		return (1);
	//jtfunction(node->heredoc_content);//
	//node->heredoc_content = expand_and_replace(&(node->heredoc_content), shell);
	// Write pre-read content to pipe
	write(pipefd[1], node->heredoc_content, ft_strlen(node->heredoc_content));
	close(pipefd[1]);

	// Redirect stdin to pipe
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);

	return (0);
}
