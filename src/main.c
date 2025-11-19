/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:53:35 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 21:21:57 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char **dup_envp(t_shell *shell, char **envp)
{
	char	**duped;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	duped = malloc(sizeof(char *) * (count + 1));
	if (!duped)
		return (NULL);
	while (i < count)
	{
		duped[i] = ft_strdup(envp[i]);
		if (!duped[i])
			return (cleanup_dup_envp(shell, i));
		i++;
	}
	duped[i] = NULL;
	return (duped);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = dup_envp(shell, envp);
	if (!shell->envp)
	{
		free(shell);
		return (NULL);
	}
	shell->last_exit_status = 0;
	shell->exit_code = 0;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->running = 1;
	set_signals();
	return (shell);
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_token	*tokens;
	char	*line;
	t_ast	*ast;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	while(!shell->exit_code)
	{
		line = readline("MS$ ");
		if (!line)					// ctrl-D (EOF)
		{
			printf("exit\n");		// remove this to save lines? yes
			break ;
		}
		history_add(line);
		tokens = lex_input(line);
		print_token_stream_colored(tokens);
		ast = parse(tokens);
		print_ast(ast);
		if (ast)
			execute_ast(ast, shell);
		free_ast(ast); // call all these in another function (norm)
		token_free_all(&tokens); // call all these in another function (norm)
		free(line); // call all these in another function (norm)
	} // lines saved 3 + 2 prints + 2 frees just nice 25.
	rl_clear_history();
	cleanup_shell(shell);
	return (shell->last_exit_status);
}
