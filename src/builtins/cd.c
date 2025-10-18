/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/10/18 23:31:47 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	builtin_cd(char **av, char **env) //int	builtin_cd(char **av, t_env **env)
{
	char	*path;
	char 	c[4096];	// current working directory
	char	*prev_pwd;
	
	// if (!av[1])
	// {
	// 	printf("Please provide a path.\n");
	// 	return (0);
	// }
	if (!av[1] || ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
		path = getenv_value(env, "HOME");
	else if (ft_strncmp(av[1], "~", ft_strlen(av[1])) == 0)
	{
		path = getenv_value(env, "OLDPWD");
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
	prev_pwd = getenv_value(env, "PWD");
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(c, sizeof(c)))
	{
		setenv_value(&env, "OLDPWD", prev_pwd);
		setenv_value(&env, "PWD", c);
	}
	printf("returning...\n");
	return (0);
}
