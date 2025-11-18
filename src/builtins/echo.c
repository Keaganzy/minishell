/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/18 15:24:32 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	is_flag_n(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');	// returns true if "-n", "-nnn" etc
}

static void skip_past_n(char **av, int *n_flag, int *i)
{
	if (av[*i] && is_flag_n(av[*i]))
	{
		*n_flag = 1;
		while (av[*i] && is_flag_n(av[*i]))
			(*i)++;
	}
}

static void free_struct (char **substr)
{
	int j;

	j = 0;
	while (substr[j])
	{
		free(substr[j]);
		substr[j] = NULL;
		j++;
	}
	free(substr);
	substr = NULL;
}

static void parse_and_echo_substrs(char *s, t_shell *shell)
{
	int		j;
	char	**substr;
	char	*val;

	j = 0;
	if (!s)
		return;
	substr = ft_split(s, ' ');
	if (substr == NULL)
		return ;
	while (substr[j])
	{
		if (j != 0)
			printf(" ");
		if (substr[j][0] == '$')
		{
			if (!substr[j][1])
				printf("$");
			else
			{
				val = getenv_value(shell->envp, &substr[j][1]);
				if (val != NULL)
					printf("%s", val);
			}
		}
		else
			printf("%s", substr[j]);
		j++;
	}
	free_struct(substr);
	return ;
}
int	ft_echo(char **av, t_shell *shell)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	skip_past_n(av, &n_flag, &i);
	while (av[i])
	{
		if (i > 1 && av[i-1] && !is_flag_n(av[i-1]))
			printf(" ");

		parse_and_echo_substrs(av[i], shell);
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
