/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   lexer_checks.c									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jotong <jotong@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/11/19 13:23:41 by jotong			#+#	#+#			 */
/*   Updated: 2025/12/21 21:32:27 by jotong		   ###   ########.fr	   */
/*																			*/
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

// static int	syntax_err(char *msg)
// {
// 	printf("%s\n", msg);
// 	return (1);
// }

int	check_syntax(char *s)
{
	if (!s || !*s || (ft_strlen(s) == 1 && *s == ' '))
		return (0);
	if (check_has_equals(s) == 0)
		return (0);
	if (check_commas_closed(s) == 0)
		return (printf("Unclosed inv commas.\n"), 0);
	return (1);
}

int	check_has_equals(char *s)
{
	char	**substr;

	if (s[0] == '"' || s[0] == '\'')
		substr = ft_split(s, s[0]);
	else
		substr = ft_split(s, ' ');
	if (!substr)
		return (0);
	if (ft_strnstr(substr[0], "=", ft_strlen(substr[0])) != NULL)
		return (printf("command \'%s\' not found.\n", substr[0]), 0);
	return (1);
}
