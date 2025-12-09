/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2025/12/09 11:09:34 by jotong           ###   ########.fr       */
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

void parse_and_echo_substrs(char **s, t_shell *shell)
{
    size_t j;
    char **substr;
    char *s_final;
    char *expanded;

	(void)shell;
    j = 0;
    s_final = NULL;
    substr = ft_split(*s, ' ');
    if (!substr)
        return;

    while (substr[j])
    {
		expanded = handle_dollars_tilde(substr[j], shell);
        expanded = handle_asterisk(expanded);
        if (s_final)
        {
            char *tmp = s_final;
            s_final = ft_strjoin(s_final, " ");
            free(tmp);

            tmp = s_final;
            s_final = ft_strjoin(s_final, expanded);
            free(tmp);
        }
        else
        {
            s_final = ft_strdup(expanded);
        }
        free(expanded);
        j++;
    }
    free_substr(substr);
    // free(*s);
    *s = s_final;
}

int	ft_echo(char **av, t_shell *shell)
{
	int		i;
	int		n_flag;
	char	*output; // the final concatenated output

	(void)shell;
	i = 1;
	n_flag = 0;
	output = NULL;
	while (av[i] && av[i][0] == '-' && is_flag_n(av[i]))
	{
		n_flag = 1;
		i++;
	}
	while (av[i])
	{
		char *expanded = ft_strdup(av[i]);
		if (!expanded)
			return (1);
		parse_and_echo_substrs(&expanded, shell);
		if (output)
		{
			char *tmp = output;
			output = ft_strjoin(output, " ");
			free(tmp);
			tmp = output;
			output = ft_strjoin(output, expanded);
			free(tmp);
		}
		else if (expanded)
		{
			output = ft_strdup(expanded);
		}
		else
			output = NULL;

		free(expanded);
		i++;
	}
	if (!output)
		output = ft_strdup("");
	if (!n_flag)
	{
		char *tmp = output;
		output = ft_strjoin(output, "\n");
		free(tmp);
	}
	write(1, output, ft_strlen(output));
	free(output);
	return (0);
}

