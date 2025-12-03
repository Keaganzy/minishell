/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 13:23:41 by jotong            #+#    #+#             */
/*   Updated: 2025/12/03 11:21:26 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

// static int	check_brackets_closed(char *s)
// {
// 	int	i;
// 	int	brackets;

// 	i = 0;
// 	brackets = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '(')
// 			brackets++;
// 		else if (s[i] == ')')
// 			brackets--;
// 		i++;
// 	}
// 	if (brackets != 0)
// 		return (0);
// 	return (1);
// }

// static int	check_empty_brackets(char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '"')
// 			while (s[i] && s[i] != '"')
// 				i++;
// 		else if (s[i] == '\'')
// 			while (s[i] && s[i] != '\'')
// 				i++;
// 		else if (s[i] == '(')
// 		{
// 			while (s[i] && s[i] == ' ')
// 				i++;
// 			if (s[i] == ')')
// 				return (0);
// 		}
// 		i++;
// 	}
// 	return (1);
// }

static int	syntax_err(char *msg)
{
	printf("%s\n", msg);
	return (0);
}

int	check_syntax(char *s)
{
	if (!s)
		return (1);
	if (check_commas_closed(s) == 0)
		return (syntax_err("Unclosed inv commas.\n"));
	return (1);
}

// this returns a pointer to the character after a substring containing inverted commas (double or single)
char	*skip_till_next_inv_comma(const char *s, size_t *i, int *q)
{
	unsigned char	inv_comma;

	inv_comma = s[*i];
	(*i)++;
	if (inv_comma == '\'')
		*q = 1;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	(*i)++;
	return (&s[*i]);
}
