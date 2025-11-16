/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/11/16 21:05:30 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	is_flag_n(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (arg[i] == '\0')
		return (0);
	while (arg[i] == 'n')
		i++;
	return (i > 1 && arg[i] == '\0');	// returns true if "-n", "-nnn" etc
}

static int	is_valid_var_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
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

static void parse_and_echo_substrs(char *s, t_shell *shell)
{
	int		j;
	char	*v_name;
	char	*v_val;
	int		v_len;

	j = 0;
	if (!s)
		return;
	// printf("parse and echo substrs called\n");
	while (s[j])
	{
		if (s[j] == '$') // && s[j+1] && s[j+1] != ' ' && s[j+1] != '$'
		{
			j++;
			if (s[j] == '?')
			{
				v_val = ft_itoa(shell->exit_code);
				printf("%s", v_val);
				free(v_val);
				j++;
				continue ;
			}
			v_len = 0;
			if (is_valid_var_char(s[j], 1))
			{
				v_len++;
				while(is_valid_var_char(s[j + v_len], 0))
					v_len++;
			}
			if (v_len > 0)
			{
				v_name = ft_strndup(&s[j], v_len);
				v_val = getenv_value(shell->env, v_name);

				if (v_val != NULL)
					printf("%s", v_val);
				free(v_name);
				// free (v_val);
				j += v_len;
				continue ;
			}
			printf("$");
		}
		else
		{
			printf("%c", s[j]);
			j++;
		}
	}
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
