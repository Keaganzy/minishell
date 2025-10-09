/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/09/30 23:38:23 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "libft.h"

int	builtin_cd(char **av, char **env) //int	builtin_cd(char **av, t_env **env)
{
	char	*path;
	char 	c[4096];	// current working directory
	char	*prev_pwd;
	size_t	av_len;
	
	av_len = ft_strlen(av[1]);
	if (!av[1] || ft_strncmp(av[1], "~", av_len) == 0)
		path = getenv_value(env, "HOME");
	else if (ft_strncmp(av[1], "~", av_len) == 0)
	{
		path = getenv_value(env, "OLDPWD");
		if (path)
			printf("%s\n", path);
	}
	else
		path = av[1];
	if (!path)
	{
		fprintf(stderr, "cd: path not set\n");
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
	return (0);
}
