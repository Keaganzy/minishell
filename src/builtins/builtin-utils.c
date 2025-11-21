/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:07:01 by jotong            #+#    #+#             */
/*   Updated: 2025/11/21 11:13:07 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "minishell.h"
#include "libft.h"

static int	check_expn_excluders(char *substr, char *d_name)
{
	char	*last_substr;
	size_t	l_d_name;
	size_t	l_last_substr;

	l_d_name = ft_strlen(d_name);
	if (!ft_strcmp(d_name, ".") || !ft_strcmp(d_name, ".."))
		return (0);
	if (substr[0] != '.' && d_name[0] == '.')
		return (0);
	last_substr = ft_strrchr(substr, '*');
	l_last_substr = ft_strlen(last_substr + 1);
	if (l_last_substr > 0 && l_d_name < l_last_substr)
		return (0);
	if (ft_strcmp(d_name + (l_d_name - l_last_substr), last_substr + 1) != 0)
		return (0);
	return (1);
}

static int	expansion_match_handlers(char *substr, char *d_name)
{
	int		start;
	int		pos;
	char	*to_match;
	int		skp_srch;

	start = 0;
	skp_srch = 0;
	if (check_expn_excluders(substr, d_name) == 0)
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
		if (ft_strnstr(d_name + skp_srch, to_match, ft_strlen(d_name)) == NULL)
			return (free(to_match), 0);
		start = pos;
		skp_srch += ft_strlen(to_match);
	}
	return (free (to_match), 1);
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
