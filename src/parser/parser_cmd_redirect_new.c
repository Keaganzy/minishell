/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_new.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:54:39 by ksng              #+#    #+#             */
/*   Updated: 2026/01/01 15:29:08 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

//parse_command

t_ast *parser_word(t_parser *p)
{
	t_ast	*node;
	int		word_count;
	int		i;

	word_count = count_words(p);
	if (word_count == 0)
		return (NULL);
	node = create_node(N_CMD);
	if (!node)
		return (NULL);
	node->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!node->argv)
		return (free(node), NULL);
	i = 0;
	while (i < word_count)
	{
		node->argv[i] = p->current->value; // node->argv[i] = ft_strdup(p->current->value);
		p->current->value = NULL;
		if (!node->argv[i])
			return (free_ast(node), NULL);
		advance(p);
		i++;
	}
	node->argv[i] = NULL;
	return (node);
}

t_ast	*parse_command(t_parser *p, t_shell *shell)
{
	t_ast	*node;

	if (match(p, T_OPEN_BRACKET))
	{
		advance(p);
		node = parse_and(p, shell);
		if (!node)
			return (NULL);
		if (!expect(p, T_CLOSE_BRACKET))
		{
			free_ast(node);
			return (NULL);
		}
		return (node);
	}
	if (match(p, T_WORD))
		return (parser_word(p));
	return (NULL);
}

//parse_redir

char	*strip_quotes(const char *str, int *flag)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			*flag = 1;
			i++;
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

t_ast *parse_one_redir(t_parser *p, t_ast *cmd, t_shell *shell)
{
	t_token		*redir_token;
	t_token		*file_token;
	t_node_type redir_type;
	t_ast		*node;
	int flag;
	int fd;

	flag = 0;
	redir_token = advance(p);
	redir_type = get_redir_type(redir_token->type);
	fd = redir_token->fd;
	// printf("-->fd %i",fd);
	file_token = expect(p, T_WORD);
	if (!file_token)
	{
		free_ast(cmd);
		return (NULL);
	}
	if (redir_type == N_HEREDOC)
	{
		file_token->value = strip_quotes(file_token->value, &flag);
		node = create_redir_node(redir_type, file_token->value, cmd, fd);
		if (node)
		{
			node->heredoc_content = read_heredoc_content(file_token->value);
			if (!flag)
				node->heredoc_content = expand_heredoc_line(node->heredoc_content, shell);
				// node->heredoc_content = expand_and_replace(&(node->heredoc_content), shell);
		}
		return (node);
	}
	return (create_redir_node(redir_type, file_token->value, cmd, fd));
}
