/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:17:56 by jotong            #+#    #+#             */
/*   Updated: 2025/10/08 17:38:12 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/wait.h>
# include <limits.h>

// # include "parser.h"
// # include "executor.h"
// # include "lexer.h"
// # include "builtins.h"
// # include "history.h"

# define PROMPT	"MS$"
# define BUFFER_SIZE 1024

typedef struct s_shell
{
	char	**env;
	int		exit_code;
	int		running;
}	t_shell;

// typedef struct s_env		// TODO: figure out if this is needed
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// }	t_env;

typedef	enum	e_node_type
{
	N_CMD,
	N_PIPE,
	N_REDIR_IN,
	N_REDIR_OUT,
	N_REDIR_APPEND,
	N_HEREDOC
}	t_node_type;

typedef enum	e_token_type
{
	T_WORD,				// Command or arg
	T_PIPE,				// |
	T_REDIR_IN,			// <
	T_REDIR_OUT,		// >
	T_REDIR_APPEND,		// >>
	T_HEREDOC,			// <<
	T_OPEN_BRACKET,		// (
	T_CLOSE_BRACKET		// )
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct	s_ast // abstract syntax tree
{
	t_node_type		type;
	char			**argv;		// for commands
	char			*filename;	// for redirections
	struct s_ast	*left;		// left side of a pipe
	struct s_ast	*right;		// right side of a pipe
}	t_ast;

// void	start_shell(t_shell *shell);
void	cleanup_shell(t_shell *shell);
void	execute_commands(t_token *tokens, t_shell *shell, char **envp, char **argv);
void	set_signals(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
char	*getenv_value(char **env, const char *key);
int		setenv_value(char ***envp, const char *key, const char *value);

#endif
