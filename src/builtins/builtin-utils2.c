/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:26:20 by jotong            #+#    #+#             */
/*   Updated: 2025/11/26 14:27:21 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	handle_dollars_tilde(char *substr, t_shell *shell, size_t *k, char **s_final)
{
	char	*val;

	if (substr[*k] == '~' && substr[*k + 1] == '\0')
	{
		*s_final = ft_strjoin_and_free(s_final, getenv_value(shell->envp, "HOME"), 1); // printf("%s", getenv_value(shell->envp, "HOME"));
		return (1);
	}
	if (substr[*k] != '$')
		return (0);
	if (!substr[*k + 1])
		*s_final = ft_strjoin_and_free(s_final, "$", 1); //  printf("$");
	else
	{
		val = getenv_value(shell->envp, &substr[*k + 1]);
		if (val != NULL)
		{
			*s_final = ft_strjoin_and_free(s_final, val, 1); // printf("%s", val);
			return (1);
		}
	}
	return (1);
}

char	*ft_strjoin_char_and_free(char **new_s, char c, int to_free)
{
	char	*tmp;
	size_t	s_len;

	if (!(*new_s) && !c)
		return (NULL);
	if (!c)
		return (*new_s);
	if (!(*new_s))
	{
		*new_s = malloc(2 * sizeof(char));
		if (!(*new_s))
			return (NULL);
		ft_memcpy(*new_s, &c, 1);
		(*new_s)[1] = '\0';
		return (*new_s);
	}
	tmp = *new_s;
	s_len = ft_strlen(*new_s);
	*new_s = malloc((s_len + 2) * sizeof (char));
	if (!(*new_s))
		return (NULL);
	ft_memcpy(*new_s, tmp, ft_strlen(tmp));
	(*new_s)[s_len] = c;
	(*new_s)[s_len + 1] = '\0';
	if (to_free == 1 && tmp)
		free(tmp);
	return (*new_s);
}
