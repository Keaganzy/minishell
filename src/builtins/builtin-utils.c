/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:07:01 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 21:31:29 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "minishell.h"
#include "libft.h"

int	add_update_env_vars(t_shell *shell, char *av)
{
	char	*val;
	char	*key;
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (av[i] && av[i] != '=')
		i++;
	key = ft_strndup(av, i);
	val = ft_strdup(&av[i + 1]);
	if (setenv_value(&shell->envp, key, val) != 0)
		status = 1;
	free(key);
	free(val);
	return (status);
}

void	free_substr(char **substr)
{
	int	j;

	j = 0;
	while (substr[j])
	{
		free(substr[j]);
		substr[j] = NULL;
		j++;
	}
	free(substr);
	substr = NULL;
}

int	handle_dollars_tilde(char *substr, t_shell *shell, size_t *k)
{
	char	*val;

	if (substr[*k] != '$')
		return (0);
	if (!substr[*k + 1])
		printf("$");
	else
	{
		val = getenv_value(shell->envp, &substr[*k + 1]);
		if (val != NULL)
		{
			printf("%s", val);
			return (1);
		}
	}
	return (1);
}

static int	expansion_match_handlers(char *substr, char *d_name)
{
	int		start;
	int		pos;
	char	*to_match;

	start = 0;
	pos = 0;
	if (!(ft_strnstr(d_name, ".", ft_strlen(d_name)) == 0
			|| ft_strnstr(d_name, "..", ft_strlen(d_name)) == 0))
		return (0);
	while (substr[start])
	{
		while (substr[start] == '*')
			start++;
		pos = start;
		while (substr[pos] && substr[pos] != '*')
			pos++;
		if (pos == start)
			continue ;
		to_match = ft_strndup(&substr[start], pos - start);
		if (ft_strnstr(d_name, to_match, ft_strlen(d_name)) == NULL)
			return (0);
		start = pos;
	}
	return (1);
}

int	handle_asterisk(char *substr)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	if (ft_strnstr(substr, "*", ft_strlen(substr)) == NULL)
		return (0);
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (expansion_match_handlers(substr, entry->d_name))
		{
			if (count > 0)
				printf(" ");
			printf("%s", entry->d_name);
			count++;
		}
	}
	closedir(dir);
	return (1);
}
