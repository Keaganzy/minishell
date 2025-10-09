/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:32:58 by jotong            #+#    #+#             */
/*   Updated: 2025/10/03 00:47:04 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;

char 			*ft_strndup(const char *src, size_t num);
t_token			*tokenize_input(const char *input);
t_token_type	get_op_type(const char *s, size_t *i);
void			free_tokens(t_token *tokens);
char			*extract_word(const char *s, size_t *i);
t_token 		*lex_input(const char *s);

t_token			*token_new(t_token_type type, const char *value);
void			add_token_back(t_token **lst, t_token *new);
void			token_free(t_token *tok);
void			token_free_all(t_token **lst);

#endif