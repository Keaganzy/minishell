/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   builtin-utils.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jotong <jotong@student.42singapore.sg>	 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/10/21 23:07:01 by jotong			#+#	#+#			 */
/*   Updated: 2025/12/23 16:14:56 by jotong		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <dirent.h>
#include "minishell.h"
#include "libft.h"

static void	append_match(char **result, char *name, int *count)
{
	char	*tmp;

	if (*count > 0)
	{
		tmp = *result;
		*result = ft_strjoin(*result, " ");
		free(tmp);
	}
	if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0
		|| name[0] == '.')
		return ;
	tmp = *result;
	*result = ft_strjoin(*result, name);
	free(tmp);
	(*count)++;
}

static char	*scan_directory(char *substr)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;
	int				count;

	result = NULL;
	count = 0;
	dir = opendir(".");
	if (!dir)
		return (ft_strdup(substr));
	entry = readdir(dir);
	while (entry)
	{
		if (wildcard_match(substr, entry->d_name))
			append_match(&result, entry->d_name, &count);
		entry = readdir(dir);
	}
	closedir(dir);
	return (result);
}

char	*handle_asterisk(char *substr)
{
	char	*result;

	if (!ft_strnstr(substr, "*", ft_strlen(substr)))
		return (ft_strdup(substr));
	result = scan_directory(substr);
	if (!result)
		return (substr);
	return (result);
}

int	var_check(char *var)
{
	int	i;

	if (!var || !var[0])
		return (1);
	if (!(var[0] == '_' || ft_isalpha(var[0])))
		return (1);
	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (1);
		i++;
	}
	return (0);
}
