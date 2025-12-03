/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:17:56 by jotong            #+#    #+#             */
/*   Updated: 2025/12/03 14:18:27 by jotong           ###   ########.fr       */
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
#include <errno.h>

# define PROMPT	"MS$"
# define BUFFER_SIZE 1024

typedef struct s_shell
{
	char	**envp;
	int		exit_code;
	int     last_exit_status;    // $? - last exit status
    int     stdin_backup;        // Backup for stdin
    int     stdout_backup;
	int		running;
}	t_shell;

typedef	enum	e_node_type
{
	N_CMD,
	N_PIPE,
	N_REDIR_IN,
	N_REDIR_OUT,
	N_REDIR_APPEND,
	N_HEREDOC,
	N_OR,
	N_AND
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
	char			*heredoc_content; // for heredoc content
	struct s_ast	*left;		// left side of a pipe
	struct s_ast	*right;		// right side of a pipe
}	t_ast;

typedef struct s_parser
{
	t_token	*current;
	t_token	*tokens;
}	t_parser;


// void	start_shell(t_shell *shell);
void	set_signals(void);
void	sigint_handler(int sig);
char	*getenv_value(char **env, const char *key);
int		setenv_value(char ***envp, const char *key, const char *value);

// lexer module
int				check_commas_closed(char *s);
int				check_invalid_pipes(char *s);
int				is_space(char c);
t_token_type	handle_space(const char *s, size_t *i);
char			*ft_strjoin_and_free(char **new_s, char *substr, int to_free);

char 			*ft_strndup(const char *src, size_t num);
t_token			*tokenize_input(const char *input);
t_token_type	get_op_type(const char *s, size_t *i);
void			free_tokens(t_token *tokens);
char			*extract_word(const char *s, size_t *i);
t_token 		*lex_input(const char *s);
int				check_syntax(char *s);
t_token			*token_new(t_token_type type, const char *value);
void			add_token_back(t_token **lst, t_token *new);
void			token_free(t_token *tok);
void			token_free_all(t_token **lst);
void			print_tokens(t_token *token);
void			print_token_stream_colored(t_token *tokens);
char			*extract_till_next_inv_comma(const char *s, size_t *i);
char			*extract_word_with_inv_commas(const char *s, size_t *i);

// history module
# define HISTORY_FILE "~/.minishell_history"

void	history_init(void);
void	history_add(const char *line);

//executor module
int execute_ast(t_ast *root, t_shell *shell);
int execute_node(t_ast *node, t_shell *shell);
int execute_cmd(t_ast *node, t_shell *shell);
int is_builtin(t_ast *ast);
int	execute_builtin(t_ast *ast, t_shell *shell);
char	**get_paths_from_env(char **envp);
int execute_pipe(t_ast *node, t_shell *shell);
int	execute_redir(t_ast *node, t_shell *shell);
int	setup_redir_in(char *filename);
int	setup_redir_out(char *filename);
int	setup_redir_append(char *filename);
int	setup_heredoc(t_ast *node);
int	execute_logical(t_ast *node, t_shell *shell);
int	setup_redirections(t_ast *node, t_shell *shell);

// builtins module
void	free_substr (char **substr);
char 	*handle_asterisk(char *substr);
// int 	handle_dollars_tilde(char *substr, t_shell *shell, size_t *k, char **s_final);
char	*handle_dollars_tilde(char *chunk, t_shell *shell);
void 	parse_and_echo_substrs(char **s, t_shell *shell);
int		validate_identifiers(char **av);
int		ft_echo(char **av, t_shell *shell);
int		builtin_cd(char **av, t_shell *shell);
int		ft_pwd(char **av, t_shell *shell);
int		ft_env(char **av, t_shell *shell);
int		ft_exit(char **av, t_shell *shell);
int		ft_unset(char **av, t_shell *shell);
int		ft_export(char **av, t_shell *shell);
int		add_update_env_vars(t_shell *shell, char *av);
int		unsetenv_value(char ***envp, const char *key);
char	*ft_strjoin_char_and_free(char **new_s, char c, int to_free);
int 	wildcard_match(char *pattern, char *str);
char	**setup_args_arr(char **new_av, int *i, int *n_flag);

// void	print_ast(t_ast *node, int level);
void	print_ast(t_ast *node);

// parser module
t_ast *parse(t_token *tokens);
t_ast	*parse_or(t_parser *p);
t_ast	*parse_command(t_parser *p);
t_ast *parse_one_redir(t_parser *p, t_ast *cmd);
void skip_spaces(t_parser *p);
t_token	*peek(t_parser *p);
t_token	*advance(t_parser *p);
int	match(t_parser *p, t_token_type type);
t_token	*expect(t_parser *p, t_token_type type);
int is_redirection(t_token_type type);
t_node_type	get_redir_type(t_token_type type);
int	count_words(t_parser *p);
char	*read_heredoc_content(char *delimiter);
t_ast	*create_node(t_node_type type);
t_ast	*create_binary_node(t_node_type type, t_ast *left, t_ast *right);
t_ast	*create_redir_node(t_node_type type, char *filename, t_ast *cmd);
t_ast	*new_ast(t_node_type type);
void free_ast(t_ast *node);
t_ast *parser_word(t_parser *p);

// shell utils
char **cleanup_dup_envp(t_shell *shell, int index);
void	print_err(const char *s, char *av);
void cleanup_shell(t_shell *shell);
void	free_array(char **arr);

#endif
