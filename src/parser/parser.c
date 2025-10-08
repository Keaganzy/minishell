/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:22 by jotong            #+#    #+#             */
/*   Updated: 2025/10/08 16:56:52 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_ast	*parse_token(t_token *tokens)
{
	t_ast	*hd;
	t_ast	*curr;
	size_t	argc;

	hd = NULL;
	curr = hd;
	argc = 0;

	while (tokens)
	{
		if (tokens->type == T_WORD)
		{
			printf("token type = T_WORD\n");
			curr = new_ast(N_CMD);
			curr->argv = realloc(curr->argv, sizeof(char *) * (argc + 2));
			curr->argv[argc++] = ft_strdup(tokens->value);
			curr->argv[argc] = NULL;
		}
		else if (tokens->type == T_REDIR_OUT)
		{
			printf("token type = T_REDIR_OUT\n");
			curr = new_ast(N_REDIR_OUT);
			tokens = tokens->next;
			if(tokens && tokens->type == T_WORD)
				curr->filename = ft_strdup(tokens->value);
		}
		else if (tokens->type == T_PIPE)
		{
			printf("token type = T_PIPE\n");
			curr->right = new_ast(N_PIPE);
			curr = curr->right;
			argc = 0;
		}
		printf("going to next token\n");
		tokens = tokens->next;
	}
	return (hd);
}

t_ast	*parse_redirection(t_token **cur, t_ast *ast)
{
	t_ast		*redir;
	t_node_type	n_type;
	
	if ((*cur)->type == T_REDIR_IN)
		n_type = N_REDIR_IN;
	else if ((*cur)->type == T_REDIR_OUT)
		n_type = N_REDIR_OUT;
	else if ((*cur)->type == T_REDIR_OUT)
		n_type = N_REDIR_OUT;
	else if ((*cur)->type ==T_REDIR_APPEND)
		n_type = N_REDIR_APPEND;
	else
		n_type = N_HEREDOC;
	redir = new_ast(n_type);
	*cur = (*cur)->next;	// skip operator
	if (*cur && (*cur)->type == T_WORD)
	{
		redir->filename = strdup((*cur)->value);
		*cur = (*cur)->next;
	}
	redir->left = ast;
	return (redir);
}

t_ast *parse_pipeline(t_token **cur)
{
	t_ast	*left;
	t_ast	*pipe_node;

	left = parse_token(*cur);
	while (*cur && (*cur)->type == T_PIPE)
	{
		*cur = (*cur)->next;	// skip '|'
		pipe_node = new_ast(N_PIPE);
		pipe_node->right = parse_token(*cur);
		pipe_node->left = left;
		left = pipe_node;
	}
	return (left);
}
