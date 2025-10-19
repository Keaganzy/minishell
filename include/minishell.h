/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:17:56 by jotong            #+#    #+#             */
/*   Updated: 2025/10/20 00:39:25 by jotong           ###   ########.fr       */
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
# include <fcntl.h>

# define PROMPT	"MS$"
# define BUFFER_SIZE 1024

typedef struct s_shell
{
	char	**env;
	int		exit_code;
	int		running;
}	t_shell;

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
	T_CLOSE_BRACKET,	// )
	T_AND,
	T_OR,
	T_SPACE
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
void	execute_commands(t_token *tokens, t_shell *shell, char **argv);
void	set_signals(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
char	*getenv_value(char **env, const char *key);
int		setenv_value(char ***envp, const char *key, const char *value);

// lexer module
char 			*ft_strndup(const char *src, size_t num);
t_token			*tokenize_input(const char *input);
t_token_type	get_op_type(const char *s, size_t *i);
void			free_tokens(t_token *tokens);
char			*extract_word(const char *s, size_t *i);
t_token 		*lex_input(const char *s);

t_token			*token_new(t_token_type type, const char *value);
void			add_token_back(t_token **lst, t_token *new);
void			token_free(t_token *tok);
void			token_free_all(t_token **lst);
void			print_tokens(t_token *token);

int	try_builtin(char **argv, t_shell **shell);
int	builtin_cd(char **av, char **env);

// history module
# define HISTORY_FILE "~/.minishell_history"

void	history_init(void);
void	history_add(const char *line);

//executor module
int		is_builtin(t_ast *ast);
int		builtin_cd(char **av, char **env);
int		execute_ast(t_ast *root, t_shell *shell);
int		execute_builtin(t_ast *ast, t_shell *shell);
int		execute_cmd(t_ast *root, t_shell *shell);
int		execute_pipe(t_ast *root, t_shell *shell);
int		execute_redir(t_ast *root, t_shell *shell);
int		apply_redirections(t_ast *ast);
int		redirect_input(t_ast *ast);
int		redirect_output(t_ast *ast);
int		redirect_append(t_ast *ast);
int		redirect_heredoc(const char *limiter);
int		apply_redirection_to_current_fd(t_ast *root);

// builtins module
int	ft_echo(char **av, t_shell *shell);
int	ft_cd(char **av, t_shell *shell);
int	ft_pwd(char **av, t_shell *shell);
int	ft_env(char **av, t_shell *shell);
int	ft_exit(char **av, t_shell *shell);
int	ft_unset(char **av, t_shell *shell);
int	ft_export(char **av, t_shell *shell);

int	try_builtin(char **argv, t_shell **shell);
int	builtin_cd(char **av, char **env);

// parser module
t_ast	*new_ast(t_node_type type);
t_ast	*parse_token(t_token *tokens);
void	free_ast(t_ast *root);
t_ast	*parse_pipeline(t_token **curr);

#endif
