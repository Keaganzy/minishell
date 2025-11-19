/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:10:50 by jotong            #+#    #+#             */
/*   Updated: 2025/11/19 15:23:54 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char *ft_strndup(const char *src, size_t num)
{
	size_t		len;
	char		*dup;

	len = ft_strlen(src);
	if (len > num)
		len = num;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	else
	{
		ft_memcpy(dup, src, len);
		dup[len] = '\0';
		return (dup);
	}
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

t_token_type	get_op_type(const char *s, size_t *i)
{
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			*i += 2;
			return (T_HEREDOC);
		}
		(*i)++;
		return (T_REDIR_IN);
	}
	else if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			*i += 2;
			return (T_REDIR_APPEND);
		}
		(*i)++;
		return (T_REDIR_OUT);
	}
	else if (s[*i] == '(')
	{
		(*i)++;
		return (T_OPEN_BRACKET);
	}
	else if (s[*i] == ')')
	{
		(*i)++;
		return (T_CLOSE_BRACKET);
	}
	else if (s[*i] == '&' && s[*i + 1] == '&')
	{
		(*i) += 2;
		return (T_AND);
	}
	else if (s[*i] == '|' && s[*i + 1] == '|')
	{
		(*i) += 2;
		return (T_OR);
	}
	else if (s[*i] == '|')
	{
		(*i)++;
		return (T_PIPE);
	}
	else if (is_space((unsigned char)s[*i]))
	{
		(*i)++;
		while (is_space((unsigned char)s[*i]))
			(*i)++;
		return (T_SPACE);
	}
	return (T_WORD);
}

char	*extract_word(const char *s, size_t *i)
{
	size_t	start;

	start = *i;
	while (s[*i] && !is_space((unsigned char)s[*i]) && s[*i] != '|'
		&& s[*i] != '<' && s[*i] != '>' && s[*i] != '(' && s[*i] != ')'
		&& s[*i] != '"' && s[*i] != '\'')
	{
		(*i)++;
	}
	return (ft_strndup(s + start, *i - start));
}

char	*extract_till_next_inv_comma(const char *s, size_t *i)
{
	size_t			start;
	unsigned char	inv_comma;

	inv_comma = s[*i];
	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	(*i)++;
	return (ft_strndup(s + start, *i - start - 1));
}

// static void update_inv_comma_counter(int *dinv, int *inv, char c)
// {
// 	if (c == '"')
// 	{
// 		if ((*dinv) == 0)
// 			(*dinv)++;
// 		else
// 			(*dinv)--;
// 	}
// 	else
// 	{
// 		if ((*inv) == 0)
// 			(*inv)++;
// 		else
// 			(*inv)--;
// 	}
// 	return ;
// }

static void	ft_strjoin_and_free(char **new_s, char *substr)
{
	char	*tmp;

	if (!substr)
		return;
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
// static void add_to_word(char **word, char *new_substr)
// {
// 	char	*tmp;

// 	tmp = NULL;
// 	if (*word)
// 		*word = new_substr;
// 	else
// 	{
// 		tmp = ft_strjoin(*word, new_substr);
// 		free(*word);
// 		free(new_substr);
// 		*word = tmp;
// 	}
// 	return ;
// }

t_token *lex_input(const char *s)
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
		else if (s[i] == '"' || s[i] == '\'')
		{
			if (word)
				ft_strjoin_and_free(&word, extract_till_next_inv_comma(s, &i));
			else
				word = extract_till_next_inv_comma(s, &i);
			if (!(s[i] == '\0' || s[i] == ' '))
				continue ;
			add_token_back(&tokens, token_new(T_WORD, word));
			word = NULL;
		}
		else if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '(' || s[i] == ')' || s[i] == '&')
		{
			add_token_back(&tokens, token_new(get_op_type(s, &i), NULL));
			word = NULL;
		}
		else
		{
			ft_strjoin_and_free(&word, extract_word(s, &i));
			if (s[i] == ' ' || s[i] == '\0')
			{
				add_token_back(&tokens, token_new(T_WORD, word));
				word = NULL;
			}
			else
				continue;
		}
	}
	return (tokens);
}

