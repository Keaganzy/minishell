/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:53:29 by ksng              #+#    #+#             */
/*   Updated: 2025/11/18 14:56:12 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* ************************************************************************** */
/*                           NODE TYPE TO STRING                              */
/* ************************************************************************** */

static const char	*node_type_to_string(t_node_type type)
{
	if (type == N_CMD)
		return ("CMD");
	if (type == N_PIPE)
		return ("PIPE");
	if (type == N_REDIR_IN)
		return ("REDIR_IN");
	if (type == N_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == N_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == N_HEREDOC)
		return ("HEREDOC");
	if (type == N_AND)
		return ("AND");
	if (type == N_OR)
		return ("OR");
	return ("UNKNOWN");
}

/* ************************************************************************** */
/*                           SIMPLE PRINT (Linear)                            */
/* ************************************************************************** */

static void	print_node_info(t_ast *node, int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("Node: %s\n", node_type_to_string(node->type));
	if (node->filename)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("  filename: \"%s\"\n", node->filename);
	}
	if (node->argv)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("  args: [");
		i = 0;
		while (node->argv[i])
		{
			printf("\"%s\"", node->argv[i]);
			if (node->argv[i + 1])
				printf(", ");
			i++;
		}
		printf("]\n");
	}
}

void	print_ast_simple(t_ast *node, int depth)
{
	if (!node)
		return ;
	print_node_info(node, depth);
	if (node->left)
	{
		print_ast_simple(node->left, depth + 1);
	}
	if (node->right)
	{
		print_ast_simple(node->right, depth + 1);
	}
}

/* ************************************************************************** */
/*                           TREE PRINT (Visual)                              */
/* ************************************************************************** */

static void	print_indent(int depth, int is_last[], int is_right)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		if (is_last[i])
			printf("    ");
		else
			printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (is_right)
			printf("└── ");
		else
			printf("├── ");
	}
}

static void	print_node_compact(t_ast *node)
{
	int	i;

	printf("%s", node_type_to_string(node->type));
	if (node->filename)
		printf(" (\"%s\")", node->filename);
	if (node->argv)
	{
		printf(" [");
		i = 0;
		while (node->argv[i])
		{
			printf("%s", node->argv[i]);
			if (node->argv[i + 1])
				printf(" ");
			i++;
		}
		printf("]");
	}
	printf("\n");
}

static void	print_tree_recursive(t_ast *node, int depth,
		int is_last[], int is_right)
{
	if (!node)
		return ;
	print_indent(depth, is_last, is_right);
	print_node_compact(node);
	if (node->left || node->right)
	{
		if (depth < 50)
			is_last[depth] = 0;
		if (node->left)
			print_tree_recursive(node->left, depth + 1, is_last, 0);
		if (depth < 50)
			is_last[depth] = 1;
		if (node->right)
			print_tree_recursive(node->right, depth + 1, is_last, 1);
	}
}

void	print_ast_tree(t_ast *root)
{
	int	is_last[50];
	int	i;

	if (!root)
	{
		printf("(empty tree)\n");
		return ;
	}
	i = 0;
	while (i < 50)
	{
		is_last[i] = 0;
		i++;
	}
	printf("\nAST Tree:\n");
	print_tree_recursive(root, 0, is_last, 0);
	printf("\n");
}

/* ************************************************************************** */
/*                           DETAILED PRINT                                   */
/* ************************************************************************** */

static void	print_args_detailed(char **args, int depth)
{
	int	i;
	int	j;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (j < depth)
		{
			printf("  ");
			j++;
		}
		printf("    [%d]: %s\n", i, args[i]);
		i++;
	}
}

static void	print_node_detailed(t_ast *node, int depth, char *position)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("┌─ %s ─────────────\n", position);
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("│ Type: %s\n", node_type_to_string(node->type));
	if (node->filename)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("│ Filename: \"%s\"\n", node->filename);
		if (node->heredoc_content)
		printf("│ Here_Content: \"%s\"\n", node->heredoc_content);
	}
	if (node->argv)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("│ Arguments:\n");
		print_args_detailed(node->argv, depth);
	}
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("└────────────────────\n");
}

void	print_ast_detailed(t_ast *node, int depth, char *position)
{
	if (!node)
		return ;
	print_node_detailed(node, depth, position);
	if (node->left)
		print_ast_detailed(node->left, depth + 1, "LEFT");
	if (node->right)
		print_ast_detailed(node->right, depth + 1, "RIGHT");
}

/* ************************************************************************** */
/*                           MAIN PRINT FUNCTION                              */
/* ************************************************************************** */

void	print_ast(t_ast *root)
{
	if (!root)
	{
		printf("AST is NULL\n");
		return ;
	}
	printf("\n");
	printf("=====================================\n");
	printf("         AST VISUALIZATION           \n");
	printf("=====================================\n");
	print_ast_tree(root);
	printf("=====================================\n");
	printf("       DETAILED NODE INFO            \n");
	printf("=====================================\n");
	print_ast_detailed(root, 0, "ROOT");
	printf("=====================================\n");
	printf("\n");
}

/* ************************************************************************** */
/*                           USAGE EXAMPLES                                   */
/* ************************************************************************** */

/*
** In your main or testing function, after parsing:
**
** t_token *tokens = tokenize(line);
** t_ast *ast = parse(tokens);
**
** // Print the AST
** print_ast(ast);  // Complete visualization
**
** // Or use individual functions:
** print_ast_tree(ast);      // Just the tree view
** print_ast_simple(ast, 0); // Simple indented list
** print_ast_detailed(ast, 0, "ROOT"); // Detailed boxes
**
** // Then execute
** execute_ast(ast, envp);
*/


/* ************************************************************************** */
/*                           TOKEN TYPE TO STRING                             */
/* ************************************************************************** */

static const char	*token_type_to_string(t_token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_OPEN_BRACKET)
		return ("OPEN_BRACKET");
	if (type == T_CLOSE_BRACKET)
		return ("CLOSE_BRACKET");
	if (type == T_AND)
		return ("AND");
	if (type == T_OR)
		return ("OR");
	if (type == T_SPACE)
		return ("SPACE");
	return ("UNKNOWN");
}

/* ************************************************************************** */
/*                        REPLACEMENT FOR YOUR FUNCTION                       */
/* ************************************************************************** */

void	print_tokens(t_token *token)
{
	int	i;

	i = 0;
	printf("\nToken Stream: ");
	while (token)
	{
		printf("[[%d] %s", i, token_type_to_string(token->type));
		if (token->value)
			printf(": {%s}", token->value);
		printf("]");

		if (token->next)
			printf(" ");

		token = token->next;
		i++;

		if (i > 100)
		{
			printf(" [ERROR: Too many tokens]");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COMPACT STREAM PRINT                             */
/* ************************************************************************** */

void	print_token_stream(t_token *tokens)
{
	t_token	*curr;
	int		count;

	curr = tokens;
	count = 0;
	printf("\nToken Stream: ");
	while (curr)
	{
		printf("[");
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("%s", token_type_to_string(curr->type));
		printf("]");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		count++;
		if (count > 100)
		{
			printf(" [ERROR: Too many tokens or circular list]");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           DETAILED STREAM PRINT                            */
/* ************************************************************************** */

void	print_token_stream_detailed(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	printf("\nDetailed Token Stream:\n");
	while (curr)
	{
		printf("[%d:%s", i, token_type_to_string(curr->type));
		if (curr->value && curr->value[0])
			printf(":\"%s\"", curr->value);
		printf("]");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		i++;
		if (i > 0 && i % 5 == 0 && curr)
			printf("\n    ");
		if (i > 100)
		{
			printf("\n[ERROR: Too many tokens]\n");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COLORED STREAM PRINT (Optional)                  */
/* ************************************************************************** */

static const char	*get_token_color(t_token_type type)
{
	if (type == T_WORD)
		return ("\033[0;36m");
	if (type == T_PIPE)
		return ("\033[0;35m");
	if (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC)
		return ("\033[0;33m");
	if (type == T_OPEN_BRACKET || type == T_CLOSE_BRACKET)
		return ("\033[0;32m");
	if (type == T_AND || type == T_OR)
		return ("\033[0;31m");
	return ("\033[0m");
}

void	print_token_stream_colored(t_token *tokens)
{
	t_token		*curr;
	int			count;
	const char	*color;

	curr = tokens;
	count = 0;
	printf("\nToken Stream (Colored):\n");
	while (curr)
	{
		color = get_token_color(curr->type);
		printf("%s[", color);
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("%s", token_type_to_string(curr->type));
		printf("]\033[0m");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		count++;
		if (count > 100)
		{
			printf("\033[0;31m [ERROR: Too many tokens]\033[0m");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COMPACT ONE-LINER                                */
/* ************************************************************************** */

void	print_token_stream_compact(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("<%s>", token_type_to_string(curr->type));

		if (curr->next)
			printf(" ");

		curr = curr->next;
	}
	printf("\n");
}

/* ************************************************************************** */
/*                           DIAGNOSTIC PRINT                                 */
/* ************************************************************************** */

void	print_token_diagnostic(t_token *tokens)
{
	t_token	*curr;
	int		i;
	int		word_count;
	int		space_count;
	int		operator_count;

	curr = tokens;
	i = 0;
	word_count = 0;
	space_count = 0;
	operator_count = 0;
	printf("\n=== TOKEN DIAGNOSTIC ===\n");
	printf("Stream: ");
	while (curr && i < 100)
	{
		if (curr->value && curr->value[0])
			printf("[%s]", curr->value);
		else
			printf("[%s]", token_type_to_string(curr->type));
		if (curr->next)
			printf(" → ");
		if (curr->type == T_WORD)
			word_count++;
		else if (curr->type == T_SPACE)
			space_count++;
		else
			operator_count++;
		curr = curr->next;
		i++;
	}
	printf("\n");
	printf("Total tokens: %d\n", i);
	printf("  Words: %d\n", word_count);
	printf("  Spaces: %d\n", space_count);
	printf("  Operators: %d\n", operator_count);
	printf("========================\n\n");
}

/* ************************************************************************** */
/*                           USAGE EXAMPLES                                   */
/* ************************************************************************** */

/*
** Replace your old print_tokens() function with the new one above.
**
** Usage examples:
**
** t_token *tokens = tokenize("cat < in.txt | grep hello");
**
** // Your old function (now improved):
** print_tokens(tokens);
** Output:
** ========== TOKEN STREAM ==========
** [0] WORD: "cat" →
** [1] REDIR_IN: "<" →
** [2] WORD: "in.txt" →
** [3] PIPE: "|" →
** [4] WORD: "grep" →
** [5] WORD: "hello" [END]
** Total: 6 tokens
** ==================================
**
** // Alternative compact view:
** print_token_stream(tokens);
** Output: [cat] → [<] → [in.txt] → [|] → [grep] → [hello]
**
** // For debugging:
** print_token_diagnostic(tokens);
*/
