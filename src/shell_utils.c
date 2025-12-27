/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2025/12/27 16:55:00 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void cleanup_shell(t_shell *shell)
{
    int i;

    if (shell->envp)
    {
        i = 0;
        while (shell->envp[i])
        {
            free(shell->envp[i]);
            i++;
        }
        free(shell->envp);
		shell->envp = NULL;
    }
	if (shell->tokens)
	{
		token_free_all(&shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (shell)
		free(shell);
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
			free((*envp)[i]);
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
	free(*envp);
	*envp = new_env;
	return (0);
}

int unsetenv_value(char ***envp, const char *key)
{
    int     i;
    int     j;
    size_t  k_len;

    i = 0;
    j = 0;  // New index for compacted array
    k_len = ft_strlen(key);
    
    // Compact the array by skipping matching entries
    while ((*envp)[i])
    {
        if (ft_strncmp((*envp)[i], key, k_len) == 0
            && (*envp)[i][k_len] == '=')
        {
            // Found a match - free it and skip
            free((*envp)[i]);
        }
        else
        {
            // Keep this entry - move it to position j
            (*envp)[j] = (*envp)[i];
            j++;
        }
        i++;
    }
    
    // Null-terminate at the new end
    (*envp)[j] = NULL;
    
    // Realloc to the correct size (j elements + 1 for NULL)
    *envp = realloc(*envp, (j + 1) * sizeof(char *));
    
    return (0);
}
