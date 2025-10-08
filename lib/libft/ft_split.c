/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:03:51 by jotong            #+#    #+#             */
/*   Updated: 2025/05/27 09:04:57 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**count_words_create_result(const char *s, char c, int *word_count)
{
	int		count;
	int		in_word;
	char	**result;

	count = 0;
	in_word = 0;
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	*word_count = count;
	result = malloc((*word_count + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[*word_count] = NULL;
	return (result);
}

static void	free_split(char **arr, int count)
{
	int	i;

	i = 0;
	while (i++ < count)
		free(arr[i]);
	free(arr);
}

static char	*word_dup(const char *start, char c, char **result, int i)
{
	char	*word;
	int		len;

	len = 0;
	while (start[len] && start[len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
	{
		free_split(result, i);
		return (NULL);
	}
	ft_strlcpy(word, start, len + 1);
	return (word);
}

char	**ft_split(const char *s, char c)
{
	int		i;
	int		word_count;
	char	**result;

	i = 0;
	result = count_words_create_result(s, c, &word_count);
	if (!result)
		return (NULL);
	while (*s && i < word_count)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			result[i] = word_dup(s, c, result, i);
			if (!result[i])
				return (NULL);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	return (result);
}
