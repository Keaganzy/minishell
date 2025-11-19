/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 21:37:13 by jotong           ###   ########.fr       */
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
	return (arg[i] == '\0');
}

static void	skip_past_n(char **av, int *n_flag, int *i)
{
	if (av[*i] && is_flag_n(av[*i]))
	{
		*n_flag = 1;
		while (av[*i] && is_flag_n(av[*i]))
			(*i)++;
	}
}

static void	handle_reset_k_space(size_t *k, size_t j)
{
	*k = 0;
	if (j != 0)
		printf(" ");
	return ;
}

void	parse_and_echo_substrs(char *s, t_shell *shell)
{
	size_t	j;
	size_t	k;
	char	**substr;

	j = 0;
	substr = ft_split(s, ' ');
	if (substr == NULL)
		return ;
	while (substr[j])
	{
		handle_reset_k_space(&k, j);
		while (substr[j][k] != '\0')
		{
			if (handle_dollars_tilde(substr[j], shell, &k))
				break ;
			if (k == 0 && handle_asterisk(substr[j]))
				break ;
			printf("%c", substr[j][k]);
			k++;
		}
		j++;
	}
	free_substr(substr);
	return ;
}

int	ft_echo(char **av, t_shell *shell)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	skip_past_n(av, &n_flag, &i);
	while (av[i] != (void *)0)
	{
		if (i > 1 && av[i - 1] && !is_flag_n (av[i - 1]))
			printf(" ");
		if (av[i])
		{
			parse_and_echo_substrs(av[i], shell);
			i++;
		}
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
