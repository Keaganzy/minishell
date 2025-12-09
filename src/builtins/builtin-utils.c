/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:07:01 by jotong            #+#    #+#             */
/*   Updated: 2025/12/09 16:10:15 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "minishell.h"
#include "libft.h"

char *handle_asterisk(char *substr)
{
    DIR *dir;
    struct dirent *entry;
    char *result;
    int count;

    result = NULL;
    count = 0;
    if (!ft_strnstr(substr, "*", ft_strlen(substr)))
        return ft_strdup(substr); // no wildcard
    dir = opendir(".");
    if (!dir)
        return ft_strdup(substr);
    while ((entry = readdir(dir)))
    {
        if (wildcard_match(substr, entry->d_name))
        {
            if (count > 0)
            {
                char *tmp = result;
                result = ft_strjoin(result, " ");
                free(tmp);
            }
			if (ft_strcmp(entry->d_name, ".") == 0 \
                || ft_strcmp(entry->d_name, "..") == 0 \
                || entry->d_name[0] == '.')
				continue;
            char *tmp = result;
            result = ft_strjoin(result, entry->d_name);
            free(tmp);
            count++;
        }
    }
    closedir(dir);
    if (!result) // no match
        return (substr); // return ft_strdup(substr);
    return result;
}

