/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/12/09 22:34:45 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	handle_path(char *av, t_shell *shell, char **path)
{
	if (!av || ft_strcmp(av, "~") == 0)
	{
		*path = getenv_value(shell->envp, "HOME");
	}
	else if (ft_strcmp(av, "~") == 0)
	{
		*path = getenv_value(shell->envp, "OLDPWD");
		if (*path)
			printf("%s\n", *path);
	}
	else
	{
		*path = av;
	}
}

int	builtin_cd(char **av, t_shell *shell)
{
	char	*path;
	char	c[4096];
	char	*prev_pwd;

	handle_path(av[1], shell, &path);
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
		if (prev_pwd)
			setenv_value(&(shell->envp), "OLDPWD", prev_pwd);
		setenv_value(&(shell->envp), "PWD", path);
		// free(prev_pwd);
	}
	return (0);
}

// int	builtin_cd(char **av, t_shell *shell)
// {
// 	char	*path;
// 	char	*old_pwd;
// 	char	cwd[4096];

// 	// Get the path to change to
// 	handle_path(&av[1], shell, &path);
// 	if (!path)
// 	{
// 		ft_putendl_fd("cd: path not set", 2);
// 		return (1);
// 	}

// 	// Save current PWD before changing (for OLDPWD)
// 	old_pwd = getenv_value(shell->envp, "PWD");
	
// 	// Attempt to change directory
// 	if (chdir(path) != 0)
// 	{
// 		print_err("cd", path);
// 		free(path);
// 		free(old_pwd);  // Don't forget to free!
// 		return (1);
// 	}
	
// 	// Get new current working directory
// 	if (getcwd(cwd, sizeof(cwd)))
// 	{
// 		// Update OLDPWD to previous PWD
// 		if (old_pwd)
// 			setenv_value(&(shell->envp), "OLDPWD", old_pwd);
		
// 		// Update PWD to new directory
// 		setenv_value(&(shell->envp), "PWD", cwd);
// 	}
// 	else
// 	{
// 		// getcwd failed (directory was deleted?)
// 		// Still update OLDPWD, but PWD keeps previous value or use path
// 		if (old_pwd)
// 			setenv_value(&(shell->envp), "OLDPWD", old_pwd);
// 		setenv_value(&(shell->envp), "PWD", path);
// 	}
	
// 	// free(path);
// 	// free(old_pwd);
// 	return (0);
// }
