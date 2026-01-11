/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 09:09:46 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 09:39:11 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void sort_envp_vars(char **env_var)
{
	int		i;
    int     j;
	int		count;
	char    *temp;
	
	i = 0;
	count = 0;
	while (env_var[count])
		count++;
	while (i < count - i - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_var[j], env_var[j + 1]) > 0)
			{
				temp = env_var[j];
				env_var[j] = env_var[j + 1];
				env_var[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
