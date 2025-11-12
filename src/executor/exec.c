/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:44:48 by ksng              #+#    #+#             */
/*   Updated: 2025/10/13 15:56:00 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include "parser.h"

typedef struct s_exec_context
{
	char	**envp;
	int		last_exit_status;
	int		stdin_backup;
	int		stdout_backup;
}	t_exec_context;

/* ************************************************************************** */
/*                           CONTEXT MANAGEMENT                               */
/* ************************************************************************** */

static t_exec_context	*init_context(char **envp)
{
	t_exec_context	*ctx;

	ctx = malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	ctx->envp = envp;
	ctx->last_exit_status = 0;
	ctx->stdin_backup = -1;
	ctx->stdout_backup = -1;
	return (ctx);
}

static void	backup_fds(t_exec_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
}

static void	restore_fds(t_exec_context *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
}

/* ************************************************************************** */
/*                           FORWARD DECLARATIONS                             */
/* ************************************************************************** */

static int	execute_node(t_ast_node *node, t_exec_context *ctx);
static int	execute_command(t_ast_node *node, t_exec_context *ctx);
static int	execute_pipe(t_ast_node *node, t_exec_context *ctx);
static int	execute_redir(t_ast_node *node, t_exec_context *ctx);
static int	execute_logical(t_ast_node *node, t_exec_context *ctx);

/* ************************************************************************** */
/*                           MAIN EXECUTOR                                    */
/* ************************************************************************** */

int	execute_ast(t_ast_node *root, char **envp)
{
	t_exec_context	*ctx;
	int				status;

	if (!root)
		return (0);
	ctx = init_context(envp);
	if (!ctx)
		return (1);
	status = execute_node(root, ctx);
	free(ctx);
	return (status);
}

static int	execute_node(t_ast_node *node, t_exec_context *ctx)
{
	if (!node)
		return (0);
	if (node->type == N_CMD)
		return (execute_command(node, ctx));
	if (node->type == N_PIPE)
		return (execute_pipe(node, ctx));
	if (node->type == N_REDIR_IN || node->type == N_REDIR_OUT
		|| node->type == N_REDIR_APPEND || node->type == N_HEREDOC)
		return (execute_redir(node, ctx));
	if (node->type == N_AND || node->type == N_OR)
		return (execute_logical(node, ctx));
	return (1);
}

/* ************************************************************************** */
/*                           LOGICAL OPERATORS (&&, ||)                       */
/* ************************************************************************** */

static int	execute_logical(t_ast_node *node, t_exec_context *ctx)
{
	int	left_status;

	left_status = execute_node(node->left, ctx);
	ctx->last_exit_status = left_status;
	if (node->type == N_AND)
	{
		if (left_status == 0)
			return (execute_node(node->right, ctx));
		return (left_status);
	}
	if (node->type == N_OR)
	{
		if (left_status != 0)
			return (execute_node(node->right, ctx));
		return (left_status);
	}
	return (1);
}

/* ************************************************************************** */
/*                           PIPE EXECUTION                                   */
/* ************************************************************************** */

static int	execute_pipe_child(t_ast_node *node, int *pipefd,
		t_exec_context *ctx, int is_left)
{
	if (is_left)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_node(node->left, ctx));
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_node(node->right, ctx));
	}
}

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;
	int	final_status;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		final_status = WEXITSTATUS(status2);
	else
		final_status = 1;
	return (final_status);
}

static int	execute_pipe(t_ast_node *node, t_exec_context *ctx)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid1 == 0)
		execute_pipe_child(node, pipefd, ctx, 1);
	pid2 = fork();
	if (pid2 == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid2 == 0)
		execute_pipe_child(node, pipefd, ctx, 0);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid1, pid2));
}

/* ************************************************************************** */
/*                           REDIRECTION EXECUTION                            */
/* ************************************************************************** */

static int	setup_redir_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	setup_redir_out(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	setup_redir_append(char *filename)
{
	int	fd;

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

static int	setup_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

static int	setup_redirection(t_ast_node *node)
{
	if (node->type == N_REDIR_IN)
		return (setup_redir_in(node->filename));
	if (node->type == N_REDIR_OUT)
		return (setup_redir_out(node->filename));
	if (node->type == N_REDIR_APPEND)
		return (setup_redir_append(node->filename));
	if (node->type == N_HEREDOC)
		return (setup_heredoc(node->filename));
	return (1);
}

static int	execute_redir(t_ast_node *node, t_exec_context *ctx)
{
	int	status;

	backup_fds(ctx);
	status = setup_redirection(node);
	if (status != 0)
	{
		restore_fds(ctx);
		return (status);
	}
	status = execute_node(node->left, ctx);
	restore_fds(ctx);
	return (status);
}

/* ************************************************************************** */
/*                           COMMAND EXECUTION                                */
/* ************************************************************************** */

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static int	execute_builtin(char **args, t_exec_context *ctx)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, ctx));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, ctx));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(ctx->envp));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1);
}

static char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_paths_from_env(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

static int	execute_external(char **args, t_exec_context *ctx)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		cmd_path = find_command_path(args[0], ctx->envp);
		if (!cmd_path)
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		execve(cmd_path, args, ctx->envp);
		perror(cmd_path);
		free(cmd_path);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	execute_command(t_ast_node *node, t_exec_context *ctx)
{
	int	status;

	if (!node->args || !node->args[0])
		return (0);
	if (is_builtin(node->args[0]))
		status = execute_builtin(node->args, ctx);
	else
		status = execute_external(node->args, ctx);
	ctx->last_exit_status = status;
	return (status);
}

/* ************************************************************************** */
/*                           HELPER: PATH EXTRACTION                          */
/* ************************************************************************** */

static char	**get_paths_from_env(char **envp)
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

/* ************************************************************************** */
/*                           HELPER: FREE ARRAY                               */
/* ************************************************************************** */

static void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
