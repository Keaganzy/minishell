/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_new.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:54:39 by ksng              #+#    #+#             */
/*   Updated: 2025/11/21 13:58:43 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

//parse_command

static t_ast *parse_word(t_parser *p)
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
		node->argv[i] = ft_strdup(p->current->value);
		if (!node->argv[i])
			return (free_ast(node), NULL);
		advance(p);
		i++;
	}
	node->argv[i] = NULL;
	return (node);
}

t_ast	*parse_command(t_parser *p)
{
	t_ast	*node;

	if (match(p, T_OPEN_BRACKET))
	{
		advance(p);
		node = parse_or(p);
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
		return (parse_word(p));
	return (NULL);
}

//parse_redir

t_ast *parse_one_redir(t_parser *p, t_ast *cmd)
{
	t_token		*redir_token;
	t_token		*file_token;
	t_node_type redir_type;
	t_ast		*node;

	redir_token = advance(p);
	redir_type = get_redir_type(redir_token->type);
	file_token = expect(p, T_WORD);
	if (!file_token)
	{
		free_ast(cmd);
		return (NULL);
	}
	if (redir_type == N_HEREDOC)
	{
		node = create_redir_node(redir_type, file_token->value, cmd);
		if (node)
			node->heredoc_content = read_heredoc_content(file_token->value);
		return (node);
	}
	return (create_redir_node(redir_type, file_token->value, cmd));
}
