/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:41:27 by jotong            #+#    #+#             */
/*   Updated: 2025/10/15 14:40:22 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	history_init(void)
{
	read_history(HISTORY_FILE);
}

void	history_add(const char *line)
{
	if (*line)
		add_history(line);
}

// void	history_save(void)
// {
// 	write_history(HISTORY_FILE);
// }