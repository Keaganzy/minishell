/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:10:50 by jotong            #+#    #+#             */
/*   Updated: 2025/12/03 11:22:53 by jotong           ###   ########.fr       */
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

char	*ft_strjoin_and_free(char **new_s, char *substr, int to_free)
{
	char	*tmp;

	if (!(*new_s) && !substr)
		return (NULL);
	if (!substr)
		return (*new_s);
	if (!(*new_s))
	{
		*new_s = substr;
		return (*new_s);
	}
	tmp = *new_s;
	*new_s = ft_strjoin(*new_s, substr);
	if ((to_free == 1 || to_free == 3) && tmp)
		free(tmp);
	if ((to_free == 2 || to_free == 3) && substr)
		free(substr);
	return (*new_s);
}

static int	parse_quotes(char **word, char *s, t_token **tokens, size_t *i)
{
	int	q;

	q = 0;
	if (s[*i] != '"' && s[*i] != '\'')
		return (0);
	if (*word)
		*word = ft_strjoin_and_free(word, extract_till_next_inv_comma(s, i, &q), 0);
	else
		*word = extract_till_next_inv_comma(s, i, &q);
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
	*word = ft_strjoin_and_free(word, extract_word(s, i), 0);
	if (s[*i] == ' ' || s[*i] == '\0' || s[*i] == '<' || s[*i] == '>'
		|| s[*i] == '(' || s[*i] == ')' || s[*i] == '&' )
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
