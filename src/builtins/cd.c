/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/30 15:17:20 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	handle_path(char **av, t_shell *shell, char **path)
{
	if (!av[1] || ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
		*path = getenv_value(shell->envp, "HOME");
	else if (ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
	{
		*path = getenv_value(shell->envp, "OLDPWD");
		if (*path)
			printf("%s\n", *path);
	}
	else
		*path = av[1];
}

int	builtin_cd(char **av, t_shell *shell)
{
	char	*path;
	char	c[4096];
	char	*prev_pwd;

	handle_path(&av[1], shell, &path);
	if (!path)
	{
		printf("cd: path not set\n");
		return (1);
	}
	prev_pwd = getenv_value(shell->envp, "PWD");
	if (chdir(path) != 0)
	{
		print_err("cd", path);
		return (1);
	}
	if (getcwd(c, sizeof(c)))
	{
		setenv_value(&(shell->envp), "OLDPWD", prev_pwd);
		setenv_value(&(shell->envp), "PWD", c);
	}
	return (0);
}
