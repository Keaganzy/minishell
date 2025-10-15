/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:10:50 by jotong            #+#    #+#             */
/*   Updated: 2025/10/15 17:43:29 by jotong           ###   ########.fr       */
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
	if (s[*i] == '|')
	{
		(*i)++;
		return (T_PIPE);
	}
	else if (s[*i] == '<')
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
		&& s[*i] != '<' && s[*i] != '>' && s[*i] != '(' && s[*i] != ')')
		(*i)++;
	return (ft_strndup(s + start, *i - start));
}

char	*extract_till_next_inv_comma(const char *s, size_t *i)
{
	size_t			start;
	size_t			end;
	unsigned char	inv_comma;

	inv_comma = s[*i];
	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	end = *i;
	if (s[*i])
		(*i)++;
	return (ft_strndup(s + start, end - start));
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf("token type: %d, ", token->type);
		if (token->value)
			printf("token value = %s (len = %zu)\n", token->value, ft_strlen(token->value));
		else
			printf("token has no value\n");
		token = token->next;
	}
	printf("done parsing and printing tokens\n");
}

t_token *lex_input(const char *s)
{
	size_t			i;
	t_token			*tokens;
	char			*word;
	char			*tmp;
	size_t			w_len;

	i = 0;
	tokens = NULL;
	while (s[i])
	{
		if (is_space((unsigned char)s[i]))
		{
			add_token_back(&tokens, token_new(get_op_type(s, &i), NULL));
		}
		else if (s[i] == '"' || s[i] == '\'')
		{
			word = extract_till_next_inv_comma(s, &i);
			w_len = ft_strlen(word) + 1;
			while (s[i] == '"' || s[i] == '\'')
			{
				tmp = extract_till_next_inv_comma(s, &i);
				w_len += ft_strlen(tmp);
				word = realloc(word, w_len);
				ft_strlcat(word, tmp, w_len);
				free(tmp);
			}
			add_token_back(&tokens, token_new(T_WORD, word));
			if (s[i] == '"' || s[i] == '\'')
				i++;
		}
		else if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '(' || s[i] == ')')
			add_token_back(&tokens, token_new(get_op_type(s, &i), NULL));
		else
		{
			word = extract_word(s, &i);
			printf("word (else flow) = %s\n", word);
			add_token_back(&tokens, token_new(T_WORD, word));
		}
	}
	// print_tokens(tokens);	// TODO: remove this later
	return (tokens);
}
