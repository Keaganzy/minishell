/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:10:50 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 17:48:39 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_token_type	handle_space(const char *s, size_t *i)
{
	(*i)++;
	while (is_space((unsigned char)s[*i]))
		(*i)++;
	return (T_SPACE);
}

static void	ft_strjoin_and_free(char **new_s, char *substr)
{
	char	*tmp;

	if (!substr)
		return ;
	if (!(*new_s))
	{
		*new_s = substr;
		return ;
	}
	tmp = *new_s;
	*new_s = ft_strjoin(*new_s, substr);
	free(tmp);
	free(substr);
}

static int	parse_quotes(char **word, char *s, t_token **tokens, size_t *i)
{
	if (s[*i] != '"' && s[*i] != '\'')
		return (0);
	if (*word)
		ft_strjoin_and_free(word, extract_till_next_inv_comma(s, i));
	else
		*word = extract_till_next_inv_comma(s, i);
	if (!(s[*i] == '\0' || s[*i] == ' '))
		return (1);
	add_token_back(tokens, token_new(T_WORD, *word));
	*word = NULL;
	return (1);
}

static int	parse_word(char **word, char *s, t_token **tokens, size_t *i)
{
	if (s[*i] == '\0')
		return (0);
	ft_strjoin_and_free(word, extract_word(s, i));
	if (s[*i] == ' ' || s[*i] == '\0')
	{
		add_token_back(tokens, token_new(T_WORD, *word));
		*word = NULL;
	}
	return (1);
}

t_token	*lex_input(const char *s)
{
	size_t			i;
	t_token			*tokens;
	char			*word;	

	i = 0;
	tokens = NULL;
	word = NULL;
	if (check_syntax((char *)s) == 0)
		return (NULL);
	while (s[i])
	{
		if (is_space((unsigned char)s[i]))
			add_token_back(&tokens, token_new(get_op_type(s, &i), NULL));
		else if (parse_quotes(&word, (char *)s, &tokens, &i))
			continue ;
		else if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '('
			|| s[i] == ')' || s[i] == '&')
		{
			add_token_back (&tokens, token_new(get_op_type(s, &i), NULL));
			word = NULL;
		}
		else if (parse_word(&word, (char *)s, &tokens, &i))
			continue ;
	}
	return (tokens);
}
