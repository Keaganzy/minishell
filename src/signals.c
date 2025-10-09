/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:52:55 by jotong            #+#    #+#             */
/*   Updated: 2025/10/03 23:28:05 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include <readline/readline.h>

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
	// write(1, "\b\b \b\b", 6);
}

void	set_signals(void)
{
	signal(SIGINT, sigint_handler);		// ctrl-C
	signal(SIGQUIT, sigquit_handler);	// ctrl-"\"
}
