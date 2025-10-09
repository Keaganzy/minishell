/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:15:24 by jotong            #+#    #+#             */
/*   Updated: 2025/10/09 16:28:48 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "history.h"
#include "builtins.h"
#include "libft.h"

int	setenv_value(char ***envp, const char *key, const char *value)
{
	int		i;
	int		j;
	size_t	key_len;
	char	*new_var;
	char	**new_env;

	key_len = ft_strlen(key);
	new_var = malloc(key_len + ft_strlen(value) + 2); // KEY=VALUE + '\0'
	j = -1;
	if (!new_var)
		return (1);
	ft_strlcpy(new_var, key, key_len + ft_strlen(value) + 2);
	new_var[key_len] = '=';
	ft_strlcpy(new_var + key_len + 1, value, ft_strlen(value) + 1);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0 && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return (0);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(new_var);
		return (1);
	}
	while (++j < i)
		new_env[j] = (*envp)[j];
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

char	*getenv_value(char **env, const char *key)
{
	int		i;
	size_t	len;

	if (!env || !key)
		return (NULL);
	len = strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return(env[i] + len + 1); // skip "KEY="
		i++;
	}
	return (NULL);
}

static int	handle_pid_err(int pid, char **argv, char **envp, t_token *tokens)
{
	if (pid == 0)
	{
		execve(argv[0], argv, envp);
		perror("execve");
		token_free_all(&tokens);
		return (-1);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-2);
	}
	else
		return (0);
}

static void	init_vars_signals(t_shell *shell, char **envp, int argc,
	char **argv)
{
	(void)argc;
	(void)argv;
	shell->env = envp;
	shell->exit_code = 0;
	shell->running = 1;
	set_signals();
}

void	execute_commands(t_token *tokens, t_shell *shell, char **envp, char **argv)
{
	t_ast	*a;
	pid_t	pid;
	int		status;
	
	printf("execute commands called \n");
	a = parse_token(tokens);
	printf("finishe parsing tokens\n");
	if (is_builtin(a))
	{
		printf("trying to execute builtin\n");
		execute_builtin(a, shell);
		return ;
	}
	printf("past execute builtin\n");
	pid = fork();
	if (handle_pid_err(pid, argv, envp, tokens) == -1)
		exit(1);	// Todo: need to update 
	printf("handle pid error did not return -1\n");
	printf("pid = %d\n", pid);
	if (pid == 0)
	{
		execute_ast(a, shell);
		exit(shell->exit_code);	//TODO might need to properly terminate and free 
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		printf("pid more than 0\n");
	}
	return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_token	*tokens;
	char	*line;

	init_vars_signals(&shell, envp, argc, argv);
	while(1)
	{
		line = readline("MS$ ");
		if (!line)					// ctrl-D (EOF)
		{
			printf("exit\n");		// remove this to save lines?
			break ;
		}
		history_add(line);
		tokens = lex_input(line);
		execute_commands(tokens, &shell, envp, argv);
		free(line);
		token_free_all(&tokens);
	}
	return (0);
}
