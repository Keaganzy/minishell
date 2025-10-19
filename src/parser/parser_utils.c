/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:29:26 by jotong            #+#    #+#             */
/*   Updated: 2025/10/19 18:14:18 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast	*new_ast(t_node_type type)
{
	t_ast *ast;

	ast = ft_calloc(1, sizeof(t_ast));
	ast->type = type;
	ast->argv = NULL;
	return (ast);
}

void	free_ast(t_ast *root)
{
	t_ast	*curr;
	t_ast	*next;

	curr = root;
	next = curr->right;
	
	while (curr)
	{
		free(curr->argv);
		free(curr);
		curr = next;
	}
	root = NULL;
}

t_ast *parse_simple_cmd(t_token **curr)
{
	t_ast	*cmd_node;
	size_t	i;
	size_t	size;
	t_token	*tmp;
	
	tmp = curr;
	size = 0;
	i = 0;
	while (tmp && tmp->type != T_PIPE && tmp->type != T_REDIR_APPEND && tmp->type != T_REDIR_OUT)
	{
		if(tmp->type == T_WORD)
			size++;
		tmp = tmp->next;
	}
	if (size == 0)
		return (NULL);
		
	cmd_node = new_ast(N_CMD);
	cmd_node->argv = (char **)malloc(sizeof(char *) * (size + 1));
	while (*curr && (*curr)->type != T_PIPE && (*curr)->type != T_REDIR_IN && (*curr)->type != T_REDIR_OUT)
	{
		if ((*curr)->type == T_WORD)
		{
			cmd_node->argv[i] = ft_strdup((*curr)->value);
			i++;
		}
		*curr = (*curr)->next;
	}
	cmd_node->argv[i] = '\0';
	return (cmd_node);
}

t_ast *parse_redirections(t_token **curr)
{
	t_ast	*node;
	t_ast	*cmd;

	cmd = parse_simple_cmd(curr);
	while (*curr && ((*curr)->type == T_REDIR_IN || (*curr)->type == T_REDIR_OUT))
	{
		node = new_ast(((*curr)->type == T_REDIR_IN) ? N_REDIR_IN : N_REDIR_OUT);
		*curr = (*curr)->next;
		if (!*curr || (*curr)->type != T_WORD)
			return (NULL); // redirection without a file
		node->filename = ft_strdup((*curr)->value);
		node->left = cmd;
		cmd = node;
		*curr = (*curr)->next;
	}
	return (cmd);
}

t_ast *parse_pipeline(t_token **curr)
{
	t_ast	*n_left;
	t_ast	*n_pipe;

	n_left = parse_redirections(curr);
	while (*curr && (*curr)->type == T_PIPE)
	{
		*curr = (*curr)->next; // skip pipe
		n_pipe = new_ast(N_PIPE);
		n_pipe->left = n_left;
		n_pipe->right = parse_redirections(curr); // get the right side of the pipe.
		if (!n_pipe->right)
			return (NULL);
		n_left = n_pipe; // new pipe node becomes the root for the next iteration
	}
	return (n_left);
}
