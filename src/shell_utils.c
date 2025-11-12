/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/11/12 17:25:13 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

// Helper function to convert the enum type to a readable string
// static char *get_node_type_name(t_node_type type)
// {
//     if (type == N_CMD)
//         return "COMMAND";
//     if (type == N_PIPE)
//         return "PIPE";
//     if (type == N_REDIR_OUT)
//         return "REDIR_OUT (>)";
//     if (type == N_REDIR_IN)
//         return "REDIR_IN (<)";
//     if (type == N_HEREDOC)
//         return "HEREDOC (<<)";
//     if (type == N_REDIR_APPEND)
//         return "REDIR_APPEND (>>)";
//     return "UNKNOWN_NODE";
// }

// // Recursive function to print the AST
// void print_ast(t_ast *node, int level)
// {
//     if (!node)
//         return;

//     // Print indentation based on recursion level
//     for (int i = 0; i < level; i++)
//         printf("  | ");

//     // 1. Print the current node's type
//     printf("---[%s]---\n", get_node_type_name(node->type));

//     // 2. Print Node-Specific Details (Arguments or Filenames)
//     if (node->type == N_CMD && node->argv)
//     {
//         printf("  | CMD ARGS: [");
//         for (int i = 0; node->argv[i] != NULL; i++)
//         {
//             printf("\"%s\"", node->argv[i]);
//             if (node->argv[i + 1] != NULL)
//                 printf(", ");
//         }
//         printf("]\n");
//     }
//     else if (node->filename)
//     {
//         printf("  | FILENAME: \"%s\"\n", node->filename);
//     }

//     // 3. Recurse on Left Child
//     if (node->left)
//     {
//         for (int i = 0; i < level; i++)
//             printf("  | ");
//         printf("  | Left Child:\n");
//         print_ast(node->left, level + 1);
//     }

//     // 4. Recurse on Right Child
//     if (node->right)
//     {
//         for (int i = 0; i < level; i++)
//             printf("  | ");
//         printf("  | Right Child:\n");
//         print_ast(node->right, level + 1);
//     }
// }

void cleanup_shell(t_shell *shell)
{
    int i;

    if (shell->env)
    {
        i = 0;
        while (shell->env[i])
        {
            free(shell->env[i]);
            i++;
        }
        free(shell->env);
    }
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
	*envp = new_env;
	return (0);
}

int	unsetenv_value(char ***envp, const char *key)
{
	int		i;
	int		freed;
	size_t	k_len;

	i = 0;
	freed = -1;
	k_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, k_len) == 0
			&& (*envp)[i][k_len] == '=')
		{
			free((*envp)[i]);
			freed = i;
		}
		if (freed != -1 && i >= freed)
		{
			if ((*envp)[i+1])
				(*envp)[i] = (*envp)[i+1];
			else
				(*envp)[i] = NULL;
		}
		i++;
	}
	(*envp) = realloc((*envp), (i - 1) * sizeof(char *));
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_printer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by your_login       #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by your_login      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		printf("    [%d]: \"%s\"\n", i, args[i]);
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
