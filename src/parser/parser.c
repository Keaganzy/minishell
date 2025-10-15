/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:22 by jotong            #+#    #+#             */
/*   Updated: 2025/10/15 00:01:23 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_ast	*parse_token(t_token *tokens)
{
	t_ast	*hd;
	t_ast	*curr;
	size_t	size; // number of spaces malloced in the argv
	int		i;		// curr position in the argv
	t_token	*tmp;

	hd = NULL;
	curr = hd;
	i = 0;
	while (tokens)
	{
		if (tokens->type == T_WORD)
		{
			size = 2;
			i = 0;
			curr = new_ast(N_CMD);
			curr->argv = malloc(sizeof(char *) * (size));
			curr->argv[i] = ft_strdup(tokens->value); // todo: i think this part is wrong, if the word is a function then argv needs to pull from next tokens.
			i++;
			hd = curr;
			tmp = tokens;
			while (tmp->next && (tmp->next->type == T_WORD || tmp->next->type == T_SPACE)) // this section is needed to populate the argv array with all props
			{
				size++;
				curr->argv = realloc(curr->argv, sizeof(char *) * size);
				if (tmp->next->type == T_SPACE)
				{
					if (i > 1)
					{
						curr->argv[i] = malloc(sizeof(2));
						curr->argv[i][0] = ' ';
						curr->argv[i][1] = '\0';
						i++;
					}
				}
				else
				{
					curr->argv[i] = ft_strdup(tmp->next->value);
					i++;
				}
				tmp = tmp->next;
			}
			tokens = tmp;
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
			size = 0;
		}
		else if (tokens->type == T_SPACE && i > 0) // TODO: i dont think this logic (in this function is correct, need to account for space in other scenarios)
		{
			printf("token type = T_SPACE\n");
			curr->argv[i] = realloc(curr->argv[i], sizeof(char *) * (size + 1));
			curr->argv[i][0] = ' ';
			curr->argv[i][1] = '\0';
			// arr_len += 1;
			i++;
		}
		else if (tokens->type == T_AND)
		{
			printf("token type = T_AND\n");
			// TODO: Do something
		}
		else if (tokens->type == T_OR)
		{
			printf("token type = T_OR\n");
			// TODO: Do something
		}
		if (hd == NULL)
			hd = curr;
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
