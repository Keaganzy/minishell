/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:40:28 by jotong            #+#    #+#             */
/*   Updated: 2025/09/21 23:32:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include "minishell.h"

# define HISTORY_FILE "~/.minishell_history"

void	history_init(void);
void	history_add(const char *line);
// void	history_save(void);

#endif