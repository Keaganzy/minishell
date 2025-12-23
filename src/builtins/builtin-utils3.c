/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:34:29 by jotong            #+#    #+#             */
/*   Updated: 2025/12/23 16:29:51 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static char	**split_chunks(char *str, int *count)
{
	char	**res;
	int		i;
	int		j;
	int		c;

	res = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!res)
		return (NULL);
	i = 0;
	c = 0;
	while (str[i])
	{
		while (str[i] == '*')
			i++;
		if (!str[i])
			break ;
		j = i;
		while (str[j] && str[j] != '*')
			j++;
		res[c] = ft_strndup(str + i, j - i);
		if (!res[c])
			return (free_tab(res), NULL);
		c++;
		i = j;
	}
	res[c] = NULL;
	*count = c;
	return (res);
}

int	wildcard_match(char *pattern, char *str)
{
	char	**chunks;
	int		n;
	int		pos;
	int		start_star;
	int		end_star;

	start_star = (pattern[0] == '*');
	end_star = (pattern[ft_strlen(pattern) - 1] == '*');
	chunks = split_chunks(pattern, &n);
	if (!chunks)
		return (0);
	pos = 0;
	if (match_first(str, chunks, &pos, start_star))
		return (free_tab(chunks), 0);
	if (match_middle(str, chunks, n, &pos))
		return (free_tab(chunks), 0);
	if (match_last(str, chunks, n, end_star))
		return (free_tab(chunks), 0);
	free_tab(chunks);
	return (1);
}
