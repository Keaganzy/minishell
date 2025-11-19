/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 13:23:41 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 16:04:36 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	check_brackets_closed(char *s)
{
	int	i;
	int	brackets;

	i = 0;
	brackets = 0;
	while (s[i])
	{
		if (s[i] == '(')
			brackets++;
		else if(s[i] == ')')
			brackets--;
		i++;
	}
	if (brackets != 0)
		return 0;
	return 1;
}

static int	check_empty_brackets(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"')
			while (s[i] && s[i] != '"')
				i++;
		else if (s[i] == '\'')
			while (s[i] && s[i] != '\'')
				i++;
		else if (s[i] == '(')
		{
			while (s[i] && s[i] == ' ')
				i++;
			if (s[i] == ')')
				return (0);
		}
		i++;
	}
	return (1);
}

static int	syntax_err(char *msg)
{
	printf("%s\n", msg);
	return (0);
}

int	check_syntax(char *s)
{
	if (!s)
		return (1);
	if (check_brackets_closed(s) == 0)
		return(syntax_err("Unexpected syntax error near the brackets.\n"));
	else if (check_commas_closed(s) == 0)
		return(syntax_err("Unclosed inv commas.\n"));
	else if (check_empty_brackets(s) == 0 || check_invalid_pipes(s) == 0)
		return(syntax_err("Syntax error."));
	return (1);
}
