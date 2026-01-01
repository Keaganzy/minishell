/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:52:55 by jotong            #+#    #+#             */
/*   Updated: 2026/01/01 21:46:47 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"
#include "libft.h"

volatile sig_atomic_t g_sigint_received;

// static void	sigint_handler(int sig, siginfo_t *action)
// {
// 	pid_t	pid;
	
// 	(void)sig;
// 	pid = getpid();
// 	// printf("pid is %d\n", pid);
// 	// printf("ctrlc is called\n");
// 	g_sigint_received = 130;
// 	if (sig == SIGINT && action->si_pid != 0)
// 	{
// 		write(1, "\n", 1);
// 		// rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// 	else
// 		exit(130);
	
	
// }

// void	set_signals(void)
// {
// 	struct sigaction sa;
// 	// struct sigaction sa_child;
	
// 	ft_bzero(&sa, sizeof(sa));  // Pass the size of the struct
// 	sa.sa_handler = sigint_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;

//     // sa_child.sa_handler = sigint_handler_child;
//     // sigemptyset(&sa_child.sa_mask);
//     // sa_child.sa_flags = 0;
//     // sigaction(SIGINT, &sa_child, NULL);
	
// 	signal(SIGQUIT, SIG_IGN); // ctrl-"\"
	
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction failed");
// 		exit(EXIT_FAILURE);
// 	}
// }
volatile sig_atomic_t g_sigint_received;

static void	sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	g_sigint_received = 130;
	
	if (sig == SIGINT && info->si_pid != 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// else
	// 	exit(130);
}

void	set_signals(void)
{
	struct sigaction sa;
	
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = sigint_handler;  // Use sa_sigaction, not sa_handler
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;  // Add this flag!
	
	signal(SIGQUIT, SIG_IGN);
	
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction failed");
		exit(EXIT_FAILURE);
	}
}
