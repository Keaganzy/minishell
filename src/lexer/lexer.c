/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:10:50 by jotong            #+#    #+#             */
/*   Updated: 2025/11/12 20:17:14 by jotong           ###   ########.fr       */
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
		&& s[*i] != '\'' && s[*i] != '"')
		(*i)++;
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
	return (ft_strndup(s + start, *i - start));
}

/* ************************************************************************** */
/*                           TOKEN TYPE TO STRING                             */
/* ************************************************************************** */

static const char	*token_type_to_string(t_token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_OPEN_BRACKET)
		return ("OPEN_BRACKET");
	if (type == T_CLOSE_BRACKET)
		return ("CLOSE_BRACKET");
	if (type == T_AND)
		return ("AND");
	if (type == T_OR)
		return ("OR");
	if (type == T_SPACE)
		return ("SPACE");
	return ("UNKNOWN");
}

/* ************************************************************************** */
/*                        REPLACEMENT FOR YOUR FUNCTION                       */
/* ************************************************************************** */

void	print_tokens(t_token *token)
{
	int	i;

	i = 0;
	printf("\nToken Stream: ");
	while (token)
	{
		printf("[[%d] %s", i, token_type_to_string(token->type));
		if (token->value)
			printf(": {%s}", token->value);
		printf("]");

		if (token->next)
			printf(" ");

		token = token->next;
		i++;

		if (i > 100)
		{
			printf(" [ERROR: Too many tokens]");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COMPACT STREAM PRINT                             */
/* ************************************************************************** */

void	print_token_stream(t_token *tokens)
{
	t_token	*curr;
	int		count;

	curr = tokens;
	count = 0;
	printf("\nToken Stream: ");
	while (curr)
	{
		printf("[");
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("%s", token_type_to_string(curr->type));
		printf("]");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		count++;
		if (count > 100)
		{
			printf(" [ERROR: Too many tokens or circular list]");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           DETAILED STREAM PRINT                            */
/* ************************************************************************** */

void	print_token_stream_detailed(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	printf("\nDetailed Token Stream:\n");
	while (curr)
	{
		printf("[%d:%s", i, token_type_to_string(curr->type));
		if (curr->value && curr->value[0])
			printf(":\"%s\"", curr->value);
		printf("]");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		i++;
		if (i > 0 && i % 5 == 0 && curr)
			printf("\n    ");
		if (i > 100)
		{
			printf("\n[ERROR: Too many tokens]\n");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COLORED STREAM PRINT (Optional)                  */
/* ************************************************************************** */

static const char	*get_token_color(t_token_type type)
{
	if (type == T_WORD)
		return ("\033[0;36m");
	if (type == T_PIPE)
		return ("\033[0;35m");
	if (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC)
		return ("\033[0;33m");
	if (type == T_OPEN_BRACKET || type == T_CLOSE_BRACKET)
		return ("\033[0;32m");
	if (type == T_AND || type == T_OR)
		return ("\033[0;31m");
	return ("\033[0m");
}

void	print_token_stream_colored(t_token *tokens)
{
	t_token		*curr;
	int			count;
	const char	*color;

	curr = tokens;
	count = 0;
	printf("\nToken Stream (Colored):\n");
	while (curr)
	{
		color = get_token_color(curr->type);
		printf("%s[", color);
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("%s", token_type_to_string(curr->type));
		printf("]\033[0m");

		if (curr->next)
			printf(" → ");

		curr = curr->next;
		count++;
		if (count > 100)
		{
			printf("\033[0;31m [ERROR: Too many tokens]\033[0m");
			break ;
		}
	}
	printf("\n\n");
}

/* ************************************************************************** */
/*                           COMPACT ONE-LINER                                */
/* ************************************************************************** */

void	print_token_stream_compact(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("<%s>", token_type_to_string(curr->type));

		if (curr->next)
			printf(" ");

		curr = curr->next;
	}
	printf("\n");
}

/* ************************************************************************** */
/*                           DIAGNOSTIC PRINT                                 */
/* ************************************************************************** */

void	print_token_diagnostic(t_token *tokens)
{
	t_token	*curr;
	int		i;
	int		word_count;
	int		space_count;
	int		operator_count;

	curr = tokens;
	i = 0;
	word_count = 0;
	space_count = 0;
	operator_count = 0;
	printf("\n=== TOKEN DIAGNOSTIC ===\n");
	printf("Stream: ");
	while (curr && i < 100)
	{
		if (curr->value && curr->value[0])
			printf("[%s]", curr->value);
		else
			printf("[%s]", token_type_to_string(curr->type));
		if (curr->next)
			printf(" → ");
		if (curr->type == T_WORD)
			word_count++;
		else if (curr->type == T_SPACE)
			space_count++;
		else
			operator_count++;
		curr = curr->next;
		i++;
	}
	printf("\n");
	printf("Total tokens: %d\n", i);
	printf("  Words: %d\n", word_count);
	printf("  Spaces: %d\n", space_count);
	printf("  Operators: %d\n", operator_count);
	printf("========================\n\n");
}

/* ************************************************************************** */
/*                           USAGE EXAMPLES                                   */
/* ************************************************************************** */

/*
** Replace your old print_tokens() function with the new one above.
**
** Usage examples:
**
** t_token *tokens = tokenize("cat < in.txt | grep hello");
**
** // Your old function (now improved):
** print_tokens(tokens);
** Output:
** ========== TOKEN STREAM ==========
** [0] WORD: "cat" →
** [1] REDIR_IN: "<" →
** [2] WORD: "in.txt" →
** [3] PIPE: "|" →
** [4] WORD: "grep" →
** [5] WORD: "hello" [END]
** Total: 6 tokens
** ==================================
**
** // Alternative compact view:
** print_token_stream(tokens);
** Output: [cat] → [<] → [in.txt] → [|] → [grep] → [hello]
**
** // For debugging:
** print_token_diagnostic(tokens);
*/

static void update_inv_comma_counter(int *dinv, int *inv, char c)
{
	if (c == '"')
	{
		if ((*dinv) == 0)
			(*dinv)++;
		else
			(*dinv)--;
	}	
	else
	{
		if ((*inv) == 0)
			(*inv)++;
		else
			(*inv)--;
	}
	return ;
}

t_token *lex_input(const char *s)
{
	size_t			i;
	t_token			*tokens;
	t_token_type	t;
	char			*word;
	int				d_inv_comma;
	int				inv_comma;

	i = 0;
	inv_comma = 0;
	d_inv_comma = 0;
	tokens = NULL;
	while (s[i])
	{
		if (is_space((unsigned char)s[i]))
		{
			t = get_op_type(s, &i);
			add_token_back(&tokens, token_new(t, NULL));
		}
		else if (s[i] == '"' || s[i] == '\'')
		{
			// add handle comma funct here
			update_inv_comma_counter(&d_inv_comma, &inv_comma, s[i]);
			word = extract_till_next_inv_comma(s, &i);
			add_token_back(&tokens, token_new(T_WORD, word));
			// free(word);	// TODO: need to free during cleanup
			if (s[i] == '"' || s[i] == '\'')
			{
				update_inv_comma_counter(&d_inv_comma, &inv_comma, s[i]);
				i++;
			}
		}
		else if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '(' || s[i] == ')' || s[i] == '&')
		{
			t = get_op_type(s, &i);
			// printf("t = %d\n", t);
			add_token_back(&tokens, token_new(t, NULL));
		}
		else
		{
			word = extract_word(s, &i);
			add_token_back(&tokens, token_new(T_WORD, word));
			// free(word);
		}
	}
	printf("inv_comma = %d\n", inv_comma);
	printf("d_inv_comma = %d\n", d_inv_comma);
	if (inv_comma != 0 || d_inv_comma != 0)
	{
		printf("Brackets are not closed!\n");
		token_free_all(&tokens);
		return (NULL);
	}
	return (tokens);
}
