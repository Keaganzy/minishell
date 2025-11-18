/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/18 15:02:40 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	builtin_cd(char **av, t_shell *shell) //int	builtin_cd(char **av, t_env **env)
{
	char	*path;
	char 	c[4096];	// current working directory
	char	*prev_pwd;

	if (!av[1] || ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
		path = getenv_value(shell->envp, "HOME");
	else if (ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
	{
		path = getenv_value(shell->envp, "OLDPWD");
		if (path)
			printf("%s\n", path);
	}
	else
		path = av[1];
	if (!path)
	{
		printf("cd: path not set\n");
		return (1);
	}
	prev_pwd = getenv_value(shell->envp, "PWD");
	if (chdir(path) != 0)
	{
		// perror("cd");
		print_err("cd", path);
		return (1);
	}
	if (getcwd(c, sizeof(c)))
	{
		setenv_value(&(shell->envp), "OLDPWD", prev_pwd);
		setenv_value(&(shell->envp), "PWD", c);
	}
	// printf("returning...\n");
	return (0);
}
