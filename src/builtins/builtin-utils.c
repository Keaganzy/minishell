/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:07:01 by jotong            #+#    #+#             */
/*   Updated: 2025/12/21 21:31:14 by jotong           ###   ########.fr       */
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
    if (!result)
        return (substr); // return ft_strdup(substr);
    return result;
}

int var_check(char *var)
{
    int i;

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
