/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:53:35 by jotong            #+#    #+#             */
/*   Updated: 2025/11/12 17:42:34 by ksng             ###   ########.fr       */
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

static void	init_vars_signals(t_shell *shell, char **envp, int argc,
	char **argv)
{
	(void)argc;
	(void)argv;
	shell->env = dup_envp(shell, envp);
	shell->exit_code = 0;
	shell->running = 1;
	set_signals();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_token	*tokens;
	char	*line;
	t_ast	*a;

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
		print_token_stream_colored(tokens);
		//print_tokens(tokens);
		// a = parse_pipeline(&tokens);
		a = parse(tokens);
		// print_ast(a, 3);
		print_ast(a);
		execute_ast(a, &shell);
		token_free_all(&tokens);
		free_ast(a);
		free(line);
		rl_clear_history();
		cleanup_shell(&shell);
	}
	return (0);
}
