/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:21:47 by jotong            #+#    #+#             */
/*   Updated: 2025/09/30 22:32:50 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	redirect_input(t_ast *ast)
{
	int	fd;
	
	fd = open(ast->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(ast->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect_output(t_ast *ast)
{
	int	fd;

	fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(ast->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redirect_append(t_ast *ast)
{
	int	fd;

	fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(ast->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redirect_heredoc(const char *limiter)
{
	int		pipe_fd[2];
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	printf("heredoc> ");
	while (getline(&line, &len, stdin) != -1)
	{
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')	// remove newline
			line[len - 1] = '\0';
		if (strcmp(line, limiter) == 0)	// stop if delimiter is reached
			break ;
		write(pipe_fd[1], line, len);
		write(pipe_fd[1], "\n", 1);
		printf("heredoc> ");
	}
	free(line);
	close(pipe_fd[1]);	// close write end, redirect read end to stdin
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (0);
}

int	apply_redirections(t_ast *ast)
{
	while (ast)
	{
		if (ast->type == N_REDIR_IN)
			redirect_input(ast);		// dup2 + open file
		else if (ast->type == N_REDIR_OUT)
			redirect_output(ast);
		else if (ast->type == N_REDIR_APPEND)
			redirect_append(ast);
		else if (ast->type == N_HEREDOC)
			redirect_heredoc(ast->filename);
		ast = ast->right;
	}
	return (0);
}
