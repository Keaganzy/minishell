/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:21:47 by jotong            #+#    #+#             */
/*   Updated: 2025/10/22 19:05:33 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_input(t_ast *ast)
{
	(void)ast;
// 	int	fd;
	
// 	fd = open(ast->filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror(ast->filename);
// 		return (-1);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
	return (0);
}

int	redirect_output(t_ast *ast)
{
	(void)ast;
	// int	fd;

	// fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// if (fd < 0)
	// {
	// 	perror(ast->filename);
	// 	return (-1);
	// }
	// dup2(fd, STDOUT_FILENO);
	// close(fd);
	return (0);
}

int	redirect_append(t_ast *ast)
{
	(void)ast;
	// int	fd;

	// fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// if (fd < 0)
	// {
	// 	perror(ast->filename);
	// 	return (-1);
	// }
	// dup2(fd, STDOUT_FILENO);
	// close(fd);
	return (0);
}

int	redirect_heredoc(const char *limiter)
{
	(void)limiter;
	// int		pipe_fd[2];
	// char	*line;
	// size_t	len;

	// line = NULL;
	// len = 0;
	// if (pipe(pipe_fd) == -1)
	// {
	// 	perror("pipe");
	// 	return (-1);
	// }
	// printf("heredoc> ");
	// while (getline(&line, &len, stdin) != -1)
	// {
	// 	len = strlen(line);
	// 	if (len > 0 && line[len - 1] == '\n')	// remove newline
	// 		line[len - 1] = '\0';
	// 	if (strcmp(line, limiter) == 0)	// stop if delimiter is reached
	// 		break ;
	// 	write(pipe_fd[1], line, len);
	// 	write(pipe_fd[1], "\n", 1);
	// 	printf("heredoc> ");
	// }
	// free(line);
	// close(pipe_fd[1]);	// close write end, redirect read end to stdin
	// dup2(pipe_fd[0], STDIN_FILENO);
	// close(pipe_fd[0]);
	return (0);
}

int	apply_redirections(t_ast *ast)
{
	(void)ast;
	// while (ast)
	// {
	// 	if (ast->type == N_REDIR_IN)
	// 		redirect_input(ast);		// dup2 + open file
	// 	else if (ast->type == N_REDIR_OUT)
	// 		redirect_output(ast);
	// 	else if (ast->type == N_REDIR_APPEND)
	// 		redirect_append(ast);
	// 	else if (ast->type == N_HEREDOC)
	// 		redirect_heredoc(ast->filename);
	// 	ast = ast->right;
	// }
	return (0);
}

int	execute_redir(t_ast *curr, t_shell *shell)
{
	printf("execute_redir\n");
	if (apply_redirection_to_curr_fd(curr) == -1)
		return (1);
	return (execute_ast(curr->left, shell));
}

int	apply_redirection_to_curr_fd(t_ast *curr) // TODO: need to figure out input arg (root / curr node?)
{
	int	file_fd;
	int	target_fd;

	printf("apply redir to curr fd called\n");
	if (!curr || !curr->filename)
		return(1);	// missing filename
	printf("filename: %s!\n", curr->filename);
	printf("curr type is %d\n", curr->type);
	if (curr->type == N_REDIR_OUT)
	{
		file_fd = open(curr->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		target_fd = STDOUT_FILENO;
		printf("file_fd = %d, target_fd = %d\n", file_fd, target_fd);
	}
	else if (curr->type == N_REDIR_IN)
	{
		file_fd = open(curr->filename, O_RDONLY);
		target_fd = STDIN_FILENO;
	}
	else
	{
		// TODO: Handle heredoc, N_REDIR_APPEND here
		return (0);
	}
	if (file_fd == -1) // fileopen error
	{
		perror(curr->filename);
		return (1);
	}
	if (dup2(file_fd, target_fd) == -1) // TODO: something in this fn is not working for >
	{
		perror("dup2 failed");
		close(file_fd);
		return (1);
	}
	close(file_fd);
	printf("function completed\n");
	return (0);
}
