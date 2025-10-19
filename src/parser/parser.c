/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:22 by jotong            #+#    #+#             */
/*   Updated: 2025/10/19 21:29:02 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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
