/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< Updated upstream:src/cleanup_utils.c
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:44:38 by jotong            #+#    #+#             */
/*   Updated: 2025/10/22 15:43:26 by jotong           ###   ########.fr       */
=======
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 22:15:56 by jotong            #+#    #+#             */
/*   Updated: 2025/10/13 15:43:59 by ksng             ###   ########.fr       */
>>>>>>> Stashed changes:include/parser.h
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

<<<<<<< Updated upstream:src/cleanup_utils.c
char **cleanup_dup_envp(t_shell *shell, int index)
{
	int		i;
	char	**duped_env;

	i = 0;
	duped_env = shell->env;
	while (i < index)
	{
		free(duped_env[i]);
		i++;
	}
	free(duped_env);
	shell->env = NULL;
	return (shell->env);
}

void cleanup_shell(t_shell *shell)
{
	int	i;
	
	i = 0;
	while (shell->env[i])
		i++;
	cleanup_dup_envp(shell, i - 1);
	return ;
}

=======
# include "minishell.h"
#include <sys/stat.h>
#include <fcntl.h>

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	char				*filename;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

typedef struct s_parser
{
	t_token	*current;
	t_token	*tokens;
}	t_parser;


#endif
>>>>>>> Stashed changes:include/parser.h
