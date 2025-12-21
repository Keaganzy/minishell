/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:16:27 by jotong            #+#    #+#             */
/*   Updated: 2025/12/21 21:32:59 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word_with_inv_commas(const char *s, size_t *i)
{
	size_t			start;
	unsigned char	inv_comma;

	inv_comma = s[*i];
	start = *i;
	(*i)++;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	(*i)++;
	return (ft_strndup(s + start, *i - start));
}
