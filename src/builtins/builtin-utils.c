/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:07:01 by jotong            #+#    #+#             */
/*   Updated: 2025/10/21 23:46:19 by jotong           ###   ########.fr       */
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
	if (setenv_value(&shell->env, key, val) != 0)
		status = 1;
	free(key);
	free(val);
	return (status);
}
