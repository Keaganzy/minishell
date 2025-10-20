/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/10/20 21:09:02 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_shell(t_shell *shell)
{
	// int	i;

	// i = 0;
	// while (shell->env[i])
	// {
	// 	free(shell->env[i]);
	// 	i++;
	// }
	// free(shell->env);
	(void)shell;
	// token_free_all(&tokens);
	return ;
}

// Helper function to convert the enum type to a readable string
static char *get_node_type_name(t_node_type type)
{
    if (type == N_CMD)
        return "COMMAND";
    if (type == N_PIPE)
        return "PIPE";
    if (type == N_REDIR_OUT)
        return "REDIR_OUT (>)";
    if (type == N_REDIR_IN)
        return "REDIR_IN (<)";
    if (type == N_HEREDOC)
        return "HEREDOC (<<)";
    if (type == N_REDIR_APPEND)
        return "REDIR_APPEND (>>)";
    return "UNKNOWN_NODE";
}

// Recursive function to print the AST
void print_ast(t_ast *node, int level)
{
    if (!node)
        return;

    // Print indentation based on recursion level
    for (int i = 0; i < level; i++)
        printf("  | ");

    // 1. Print the current node's type
    printf("---[%s]---\n", get_node_type_name(node->type));

    // 2. Print Node-Specific Details (Arguments or Filenames)
    if (node->type == N_CMD && node->argv)
    {
        printf("  | CMD ARGS: [");
        for (int i = 0; node->argv[i] != NULL; i++)
        {
            printf("\"%s\"", node->argv[i]);
            if (node->argv[i + 1] != NULL)
                printf(", ");
        }
        printf("]\n");
    }
    else if (node->filename)
    {
        printf("  | FILENAME: \"%s\"\n", node->filename);
    }
    
    // 3. Recurse on Left Child
    if (node->left)
    {
        for (int i = 0; i < level; i++)
            printf("  | ");
        printf("  | Left Child:\n");
        print_ast(node->left, level + 1);
    }

    // 4. Recurse on Right Child
    if (node->right)
    {
        for (int i = 0; i < level; i++)
            printf("  | ");
        printf("  | Right Child:\n");
        print_ast(node->right, level + 1);
    }
}
