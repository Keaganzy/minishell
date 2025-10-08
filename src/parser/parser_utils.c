/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:29:26 by jotong            #+#    #+#             */
/*   Updated: 2025/10/08 16:44:57 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
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
