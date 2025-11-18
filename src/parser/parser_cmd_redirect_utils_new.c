/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_utils_new.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:56:13 by ksng              #+#    #+#             */
/*   Updated: 2025/11/18 21:46:02 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}
t_node_type	get_redir_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (N_REDIR_IN);
	if (type == T_REDIR_OUT)
		return (N_REDIR_OUT);
	if (type == T_REDIR_APPEND)
		return (N_REDIR_APPEND);
	if (type == T_HEREDOC)
		return (N_HEREDOC);
	return (N_CMD);
}

int	count_words(t_parser *p)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = p->current;
	while (tmp)
    {
        // Skip spaces
        while (tmp && tmp->type == T_SPACE)
            tmp = tmp->next;

        // Count word
        if (tmp && tmp->type == T_WORD)
        {
            count++;
            tmp = tmp->next;
        }
        else
            break;
    }
	return (count);
}

// char	*read_heredoc_content(char *delimiter)
// {
// 	char	*line;
// 	char	*content;
// 	char	*tmp;

// 	content = ft_strdup("");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		tmp = content;
// 		content = ft_strjoin(content, line);
// 		free(tmp);
// 		tmp = content;
// 		content = ft_strjoin(content, "\n");
// 		free(tmp);
// 		free(line);
// 	}
// 	return (content);
// }
