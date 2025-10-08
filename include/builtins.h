/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:34:16 by jotong            #+#    #+#             */
/*   Updated: 2025/09/30 22:03:42 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdio.h>
# include "minishell.h"

int	ft_echo(char **av, t_shell *shell);
int	ft_cd(char **av, t_shell *shell);
int	ft_pwd(char **av, t_shell *shell);
int	ft_env(char **av, t_shell *shell);
int	ft_exit(char **av, t_shell *shell);
int	ft_unset(char **av, t_shell *shell);
int	ft_export(char **av, t_shell *shell);

int	try_builtin(char **argv, t_shell **shell);
int	builtin_cd(char **av, char **env);

#endif