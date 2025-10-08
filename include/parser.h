/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:56 by jotong            #+#    #+#             */
/*   Updated: 2025/09/21 23:38:28 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct	s_token t_token;
typedef enum	e_node_type t_node_type;
typedef	struct	s_ast t_ast;

t_ast	*new_ast(t_node_type type);
t_ast	*parse_token(t_token *tokens);
void	free_ast(t_ast *root);

#endif