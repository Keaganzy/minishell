/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:53:35 by jotong            #+#    #+#             */
/*   Updated: 2025/10/21 00:09:40 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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
		// print_tokens(tokens);
		a = parse_pipeline(&tokens);
		print_ast(a, 3);
		token_free_all(&tokens);
		free(line);
		execute_ast(a, &shell);
		
		// token_free_all(&tokens);
	}
	return (0);
}
