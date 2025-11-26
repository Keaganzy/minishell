/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:34:29 by jotong            #+#    #+#             */
/*   Updated: 2025/11/26 17:45:44 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <string.h>

/* **************************************************************** */
/*                       Helper: free **ptr                         */
/* **************************************************************** */

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

/* **************************************************************** */
/*               Split pattern by '*' into literal chunks           */
/* **************************************************************** */

static char	**split_chunks(char *pat, int *count)
{
	char	**res;
	int		i;
	int		j;
	int		c;

	res = malloc(sizeof(char *) * (ft_strlen(pat) + 1));
	if (!res)
		return (NULL);
	i = 0;
	c = 0;
	while (pat[i])
	{
		while (pat[i] == '*')
			i++;
		if (!pat[i])
			break ;
		j = i;
		while (pat[j] && pat[j] != '*')
			j++;
		res[c] = ft_strndup(pat + i, j - i);
		if (!res[c])
			return (free_tab(res), NULL);
		c++;
		i = j;
	}
	res[c] = NULL;
	*count = c;
	return (res);
}

/* **************************************************************** */
/*                 Find the chunk inside str starting at pos        */
/* **************************************************************** */

static int	find_chunk(char *str, char *chunk, int pos)
{
	size_t	len;
	int		i;

	len = ft_strlen(chunk);
	i = pos;
	while (str[i])
	{
		if (ft_strncmp(str + i, chunk, len) == 0)
			return (i + len);
		i++;
	}
	return (-1);
}

/* **************************************************************** */
/*                   Match first chunk (prefix rule)                */
/* **************************************************************** */

static int	match_first(char *str, char **chunks, int *pos, int start_star)
{
	size_t	len;

	if (start_star || !chunks[0])
		return (0);
	len = ft_strlen(chunks[0]);
	if (ft_strncmp(str, chunks[0], len) != 0)
		return (1);
	*pos = len;
	return (0);
}

/* **************************************************************** */
/*             Match middle chunks (ordered substrings rule)        */
/* **************************************************************** */

static int	match_middle(char *str, char **chunks, int n, int *pos)
{
	int		i;
	int		newpos;

	i = 1;
	while (i < n - 1)
	{
		newpos = find_chunk(str, chunks[i], *pos);
		if (newpos < 0)
			return (1);
		*pos = newpos;
		i++;
	}
	return (0);
}

/* **************************************************************** */
/*                     Match last chunk (suffix rule)               */
/* **************************************************************** */

static int	match_last(char *str, char **chunks, int n, int end_star)
{
	char	*last;
	size_t	l;
	size_t	sl;

	if (end_star || n == 0)
		return (0);
	last = chunks[n - 1];
	l = ft_strlen(last);
	sl = ft_strlen(str);
	if (sl < l)
		return (1);
	if (ft_strcmp(str + sl - l, last) != 0)
		return (1);
	return (0);
}

/* **************************************************************** */
/*                       Main wildcard function                     */
/* **************************************************************** */

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
