/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:14:20 by ksng              #+#    #+#             */
/*   Updated: 2025/11/12 13:45:03 by jotong           ###   ########.fr       */
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
