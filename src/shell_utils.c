/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/10/21 23:29:08 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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
			// free((*envp)[i]);	// TODO: check whether this needs to be uncommented
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
	// free(*envp);
	*envp = new_env;
	return (0);
}
