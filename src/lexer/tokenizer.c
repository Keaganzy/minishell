/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 23:59:23 by jotong            #+#    #+#             */
/*   Updated: 2025/12/12 20:18:43 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_token	*token_new(t_token_type type, const char *value, int fd)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	if (value)
		tok->value = (char *)value;
	else
		tok->value = NULL;
	tok->fd = fd;
	tok->next = NULL;
	return (tok);
}

void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*curr;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	curr = *lst;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

void	token_free(t_token *tok)
{
	if (!tok)
		return ;
	if (tok->value)
		free(tok->value);
	free(tok);
}

void	token_free_all(t_token **lst)
{
	t_token	*curr;
	t_token	*next;

	if (!lst)
		return ;
	curr = *lst;
	while (curr)
	{
		next = curr->next;
		token_free(curr);
		curr = next;
	}
	*lst = NULL;
}
