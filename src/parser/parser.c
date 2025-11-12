/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< Updated upstream
/*   Created: 2025/09/13 22:15:22 by jotong            #+#    #+#             */
/*   Updated: 2025/10/22 18:57:52 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast	*new_ast(t_node_type type)
{
	t_ast *ast;

	ast = ft_calloc(1, sizeof(t_ast));
	ast->type = type;
	ast->argv = NULL;
	return (ast);
}

void	free_ast(t_ast *root)
{
	t_ast	*curr;
	t_ast	*next;

	curr = root;
	next = curr->right;
	
	while (curr)
	{
		free(curr->argv);
		free(curr);
		curr = next;
	}
	root = NULL;
}

t_ast *parse_simple_cmd(t_token **curr)
{
	t_ast	*cmd_node;
	size_t	i;
	size_t	size;
	t_token	*tmp;
	
	tmp = *curr;
	size = 0;
	i = 0;
	while (tmp && tmp->type != T_PIPE && tmp->type != T_REDIR_APPEND && tmp->type != T_REDIR_OUT)
	{
		if(tmp->type == T_WORD)
			size++;
		tmp = tmp->next;
	}
	if (size == 0)
		return (NULL);
		
	cmd_node = new_ast(N_CMD);
	cmd_node->argv = (char **)malloc(sizeof(char *) * (size + 1));
	while (*curr && (*curr)->type != T_PIPE && (*curr)->type != T_REDIR_IN && (*curr)->type != T_REDIR_OUT)
	{
		if ((*curr)->type == T_WORD)
		{
			cmd_node->argv[i] = ft_strdup((*curr)->value);
			i++;
		}
		*curr = (*curr)->next;
	}
	cmd_node->argv[i] = NULL;
	return (cmd_node);
}

t_ast *parse_redirections(t_token **curr)
{
	t_ast	*node;
	t_ast	*cmd;

	cmd = parse_simple_cmd(curr);
	while (*curr && ((*curr)->type == T_REDIR_IN || (*curr)->type == T_REDIR_OUT))
	{
		node = new_ast(((*curr)->type == T_REDIR_IN) ? N_REDIR_IN : N_REDIR_OUT);
		*curr = (*curr)->next;
		if ((*curr)->type == T_SPACE)	// this skips the space after the >
			*curr = (*curr)->next;
		if (!*curr || (*curr)->type != T_WORD)
			return (NULL); // redirection without a file
		node->filename = ft_strdup((*curr)->value);
		node->left = cmd;
		cmd = node;
		*curr = (*curr)->next;
	}
	return (cmd);
}

t_ast *parse_pipeline(t_token **curr)
{
	t_ast	*n_left;
	t_ast	*n_pipe;

	n_left = parse_redirections(curr);
	while (*curr && (*curr)->type == T_PIPE)
	{
		*curr = (*curr)->next; // skip pipe
		n_pipe = new_ast(N_PIPE);
		n_pipe->left = n_left;
		n_pipe->right = parse_redirections(curr); // get the right side of the pipe.
		if (!n_pipe->right)
			return (NULL);
		n_left = n_pipe; // new pipe node becomes the root for the next iteration
	}
	return (n_left);
=======
/*   Created: 2025/10/13 15:28:49 by ksng              #+#    #+#             */
/*   Updated: 2025/10/13 15:57:39 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "parser.h"


/* ************************************************************************** */
/*                           PARSER UTILITIES                                 */
/* ************************************************************************** */

static t_token	*peek(t_parser *p)
{
	return (p->current);
}

static t_token	*advance(t_parser *p)
{
	t_token	*tmp;

	tmp = p->current;
	if (p->current)
		p->current = p->current->next;
	return (tmp);
}

static int	match(t_parser *p, t_token_type type)
{
	if (!p->current)
		return (0);
	return (p->current->type == type);
}

static t_token	*expect(t_parser *p, t_token_type type)
{
	if (!match(p, type))
		return (NULL);
	return (advance(p));
}

/* ************************************************************************** */
/*                           NODE CONSTRUCTORS                                */
/* ************************************************************************** */

static t_ast_node	*create_node(t_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->filename = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_ast_node	*create_binary_node(t_node_type type, t_ast_node *left,
		t_ast_node *right)
{
	t_ast_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->left = left;
	node->right = right;
	return (node);
}

static t_ast_node	*create_redir_node(t_node_type type, char *filename,
		t_ast_node *cmd)
{
	t_ast_node	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->filename = ft_strdup(filename);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->left = cmd;
	return (node);
}

/* ************************************************************************** */
/*                           FORWARD DECLARATIONS                             */
/* ************************************************************************** */

static t_ast_node	*parse_or(t_parser *p);
static t_ast_node	*parse_and(t_parser *p);
static t_ast_node	*parse_pipe(t_parser *p);
static t_ast_node	*parse_command(t_parser *p);
static t_ast_node	*parse_redirections(t_parser *p);
static t_ast_node	*parse_simple_cmd(t_parser *p);
static t_ast_node	*parse_primary(t_parser *p);

/* ************************************************************************** */
/*                    GRAMMAR IMPLEMENTATION (Top to Bottom)                  */
/*                                                                            */
/* Grammar:                                                                   */
/*   or_expr    -> and_expr ( "||" and_expr )*                                */
/*   and_expr   -> pipe_expr ( "&&" pipe_expr )*                              */
/*   pipe_expr  -> command ( "|" command )*                                   */
/*   command    -> redirections                                               */
/*   redirections -> simple_cmd ( redir simple_cmd )*                         */
/*   simple_cmd -> primary                                                    */
/*   primary    -> "(" or_expr ")" | WORD+                                    */
/* ************************************************************************** */

/* Parse OR expressions (||) - lowest precedence */
static t_ast_node	*parse_or(t_parser *p)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_and(p);
	if (!left)
		return (NULL);
	while (match(p, T_OR))
	{
		advance(p);
		right = parse_and(p);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_binary_node(N_OR, left, right);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

/* Parse AND expressions (&&) */
static t_ast_node	*parse_and(t_parser *p)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_pipe(p);
	if (!left)
		return (NULL);
	while (match(p, T_AND))
	{
		advance(p);
		right = parse_pipe(p);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_binary_node(N_AND, left, right);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

/* Parse PIPE expressions (|) */
static t_ast_node	*parse_pipe(t_parser *p)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_command(p);
	if (!left)
		return (NULL);
	while (match(p, T_PIPE))
	{
		advance(p);
		right = parse_command(p);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_binary_node(N_PIPE, left, right);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

/* Parse command with redirections */
static t_ast_node	*parse_command(t_parser *p)
{
	return (parse_redirections(p));
}

/* ************************************************************************** */
/*                           REDIRECTION PARSING                              */
/* ************************************************************************** */

static t_node_type	get_redir_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (N_REDIR_IN);
	if (type == T_REDIR_OUT)
		return (N_REDIR_OUT);
	if (type == T_REDIR_APPEND)
		return (N_REDIR_APPEND);
	if (type == T_HEREDOC)
		return (N_HEREDOC);
	return (N_CMD);
}

static int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}

static t_ast_node	*parse_single_redir(t_parser *p, t_ast_node *cmd)
{
	t_token		*redir_tok;
	t_token		*file_tok;
	t_node_type	redir_type;

	redir_tok = advance(p);
	redir_type = get_redir_type(redir_tok->type);
	file_tok = expect(p, T_WORD);
	if (!file_tok)
	{
		free_ast(cmd);
		return (NULL);
	}
	return (create_redir_node(redir_type, file_tok->value, cmd));
}

static t_ast_node	*parse_redirections(t_parser *p)
{
	t_ast_node	*cmd;

	cmd = parse_simple_cmd(p);
	if (!cmd)
		return (NULL);
	while (peek(p) && is_redirection(peek(p)->type))
	{
		cmd = parse_single_redir(p, cmd);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

/* ************************************************************************** */
/*                           SIMPLE COMMAND & PRIMARY                         */
/* ************************************************************************** */

static t_ast_node	*parse_simple_cmd(t_parser *p)
{
	return (parse_primary(p));
}

/* Parse parentheses or command words */
static t_ast_node	*parse_primary(t_parser *p)
{
	t_ast_node	*node;

	if (match(p, T_OPEN_BRACKET))
	{
		advance(p);
		node = parse_or(p);
		if (!node)
			return (NULL);
		if (!expect(p, T_CLOSE_BRACKET))
		{
			free_ast(node);
			return (NULL);
		}
		return (node);
	}
	if (match(p, T_WORD))
		return (parse_word_sequence(p));
	return (NULL);
}

/* ************************************************************************** */
/*                           WORD SEQUENCE PARSING                            */
/* ************************************************************************** */

static int	count_words(t_parser *p)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = p->current;
	while (tmp && tmp->type == T_WORD)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static t_ast_node	*parse_word_sequence(t_parser *p)
{
	t_ast_node	*node;
	int			word_count;
	int			i;

	word_count = count_words(p);
	if (word_count == 0)
		return (NULL);
	node = create_node(N_CMD);
	if (!node)
		return (NULL);
	node->args = malloc(sizeof(char *) * (word_count + 1));
	if (!node->args)
		return (free(node), NULL);
	i = 0;
	while (i < word_count)
	{
		node->args[i] = ft_strdup(p->current->value);
		if (!node->args[i])
			return (free_ast(node), NULL);
		advance(p);
		i++;
	}
	node->args[i] = NULL;
	return (node);
}

/* ************************************************************************** */
/*                           MAIN PARSER ENTRY POINT                          */
/* ************************************************************************** */

t_ast_node	*parse(t_token *tokens)
{
	t_parser	parser;
	t_ast_node	*ast;

	parser.tokens = tokens;
	parser.current = tokens;
	ast = parse_or(&parser);
	if (!ast)
		return (NULL);
	if (parser.current != NULL)
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
>>>>>>> Stashed changes
}
