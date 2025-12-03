/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:44:57 by jotong            #+#    #+#             */
/*   Updated: 2025/10/22 18:16:57 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_err(const char *s, char *av)
{
	write(STDERR_FILENO, s, ft_strlen(s));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	if (av)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, av, ft_strlen(av));
	}
}
