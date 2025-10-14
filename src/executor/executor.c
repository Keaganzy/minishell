/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:21:47 by jotong            #+#    #+#             */
/*   Updated: 2025/10/15 00:01:06 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"
#include "libft.h"

// focus on functions that run commands, deciding how to execute them, and calling
// the right low level functions. 

// Other responsibilities:
// manage piping between multiple commands
// call redirections handling functions before executing the command
// Wait for child processes and manage exit statuses
// Optionally, handle signals like SIGINT and SIGQUIT in the executor context

int is_builtin(t_ast *ast)
{
	int	av_len;
	
	if (!(ast || ast->argv || ast->argv[0]))
		return (0);
	av_len = ft_strlen(ast->argv[0]);
	// printf("past check 1\n");
	if (ft_strncmp(ast->argv[0], "cd", av_len) == 0)
		return (1);
	// printf("past check 2\n");
	if (ft_strncmp(ast->argv[0], "echo", av_len) == 0)
		return (1);
	// printf("past check 3\n");
	if (ft_strncmp(ast->argv[0], "env", av_len) == 0)
		return (1);
	// printf("past check 4\n");
	if (ft_strncmp(ast->argv[0], "exit", av_len) == 0)
		return (1);
	// printf("past check 5\n");	
	if (ft_strncmp(ast->argv[0], "export", av_len) == 0)
		return (1);
	// printf("past check 6\n");
	if (ft_strncmp(ast->argv[0], "unset", av_len) == 0)
		return (1);
	// printf("past check 7\n");
	if (ft_strncmp(ast->argv[0], "pwd", av_len) == 0)
		return (1);
	// printf("past check 8\n");
	return (0);
}

// static void	wait_for_all_children(void)
// {
// 	pid_t	pid;
// 	int		status;
	
// 	while ((pid = wait(&status)) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
// 		else if (WIFSIGNALED(status))
// 			printf("Child %d was killed by signal %d\n", pid, WTERMSIG(status));
// 	}
// }

int	execute_builtin(t_ast *ast, t_shell *shell)
{
	char	*cmd;
	int		arg_len;

	if (!ast || !ast->argv || !ast->argv[0])
		return (1);
	cmd = ast->argv[0];
	arg_len = ft_strlen(cmd);
	if (ft_strncmp(cmd, "cd", arg_len) == 0)
		return (builtin_cd(ast->argv, shell->env));
	else if (ft_strncmp(cmd, "echo", arg_len) == 0)
		return (ft_echo(ast->argv, shell));
	else if (ft_strncmp(cmd, "env", arg_len) == 0)
		return (ft_env(ast->argv, shell));
	else if (ft_strncmp(cmd, "pwd", arg_len) == 0)
		return (ft_pwd(ast->argv, shell));
	else if (ft_strncmp(cmd, "exit", arg_len) == 0)
		return (ft_exit(ast->argv, shell));
	else if (ft_strncmp(cmd, "export", arg_len) == 0)
		return (ft_export(ast->argv, shell));
	else if (ft_strncmp(cmd, "unset", arg_len) == 0)
		return(ft_unset(ast->argv, shell));
	return (-100);
}

int	execute_ast(t_ast *root, t_shell *shell)
{

	(void)root;
	(void)shell;
// 	int		prev_fd;
// 	t_ast	*ast;
// 	int		pipe_fd[2];
// 	pid_t	pid;
	
// 	printf("execute ast called\n");
// 	prev_fd = -1;	// for pipe chaining
// 	ast = root;
// 	while (ast)
// 	{
// 		if (ast->right)	// if there is a command to the right
// 			pipe(pipe_fd);
// 		pid = fork();
// 		if (pid == 0)	// child
// 		{
// 			if (prev_fd != -1)	// handle input from previous pipe
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 			}
// 			if (ast->right)	// handle output to the next pipe
// 			{
// 				close(pipe_fd[0]);		// close read end
// 				dup2(pipe_fd[1], STDOUT_FILENO);
// 				close(pipe_fd[1]);
// 			}
// 			apply_redirections(ast);	// handle redirections
// 			if (is_builtin(ast))
// 				execute_builtin(ast, shell);
// 			else
// 				execve(*shell->env, ast->argv, shell->env); // path should be in the first arg, args = second arg, g_env = third arg
// 			perror("execve"); // if execve fails
// 			exit(1);
// 		}
// 		if (prev_fd != -1)	// parent
// 			close(prev_fd);	// close prev pipe read end
// 		if (ast->right)
// 		{
// 			close(pipe_fd[1]);	// close write end
// 			prev_fd = pipe_fd[0];
// 		}
// 		else
// 			prev_fd = -1;
// 		ast = ast->right;
// 	}
// 	wait_for_all_children();
	return (0);
}
