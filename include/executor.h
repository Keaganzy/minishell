/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:33:01 by jotong            #+#    #+#             */
/*   Updated: 2025/10/08 00:22:28 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <errno.h>
# include <fcntl.h>

int is_builtin(t_ast *ast);
int	builtin_cd(char **av, char **env);
int	execute_ast(t_ast *root, t_shell *shell);
int	execute_builtin(t_ast *ast, t_shell *shell);
int	apply_redirections(t_ast *ast);
int	redirect_input(t_ast *ast);
int	redirect_output(t_ast *ast);
int	redirect_append(t_ast *ast);
int	redirect_heredoc(const char *limiter);

#endif