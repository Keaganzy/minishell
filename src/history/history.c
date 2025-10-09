/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:41:27 by jotong            #+#    #+#             */
/*   Updated: 2025/09/21 23:32:27 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

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