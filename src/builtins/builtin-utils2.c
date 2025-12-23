/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   builtin-utils2.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jotong <jotong@student.42singapore.sg>	 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/11/21 10:26:20 by jotong			#+#	#+#			 */
/*   Updated: 2025/12/23 16:13:51 by jotong		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*strip_outer_quotes(char *str)
{
	size_t	len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') \
		|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

int	add_update_env_vars(t_shell *shell, const char *av)
{
	char	*val;
	char	*val_stripped;
	char	*key;
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (av[i] && av[i] != '=')
		i++;
	key = ft_strndup(av, i);
	if (var_check(key) != 0)
	{
		status = 1;
		printf("export: `%s': not a valid identifier\n", key);
	}
	if (status == 0)
	{
		val = ft_strdup(&av[i + 1]);
		val_stripped = strip_outer_quotes(val);
		free(val);
		if (setenv_value(&shell->envp, key, val_stripped) != 0)
			status = 1;
		free(val_stripped);
	}
	free(key);
	return (status);
}

void	free_substr(char **substr)
{
	int	j;

	j = 0;
	while (substr[j])
	{
		free(substr[j]);
		substr[j] = NULL;
		j++;
	}
	free(substr);
	substr = NULL;
}

static size_t	expand_variable(char *chunk, size_t k,\
	char **s_final, t_shell *shell)
{
	char	*val;
	char	*v_name;
	int		v_len;

	k++;
	if (chunk[k] == '?')
	{
		val = ft_itoa(shell->last_exit_status);
		*s_final = ft_strjoin_and_free(s_final, val, 1);
		return (k + 1);
	}
	v_len = 0;
	if (ft_isalpha(chunk[k]) || chunk[k] == '_')
	{
		v_len++;
		while (ft_isalnum(chunk[k + v_len]) || chunk[k + v_len] == '_')
			v_len++;
	}
	if (v_len > 0)
	{
		v_name = ft_strndup(&chunk[k], v_len);
		val = getenv_value(shell->envp, v_name);
		if (val)
			*s_final = ft_strjoin_and_free(s_final, val, 1);
		return (free(v_name), k + v_len);
	}
	*s_final = ft_strjoin_and_free(s_final, "$", 1);
	return (k);
}

char	*handle_dollars_tilde(char *chunk, t_shell *shell)
{
	size_t	k;
	char	*s_final;
	char	*val;

	k = 0;
	s_final = NULL;
	while (chunk[k])
	{
		if (k == 0 && chunk[k] == '~' && (chunk[k + 1] == '\0' || chunk[k + 1] == '/'))
		{
			val = getenv_value(shell->envp, "HOME");
			if (val)
				s_final = ft_strjoin_and_free(&s_final, val, 1);
			k++;
			if (chunk[k] == '/')
				k++;
			continue;
		}
		if (chunk[k] == '$')
		{
			k = expand_variable(chunk, k, &s_final, shell);
			continue;
		}

		s_final = ft_strjoin_char_and_free(&s_final, chunk[k], 1);
		k++;
	}
	if (!s_final)
		return (ft_strdup(""));
	return (s_final);
}

char	*ft_strjoin_char_and_free(char **new_s, char c, int to_free)
{
	char	*tmp;
	size_t	s_len;

	if (!(*new_s) && !c)
		return (NULL);
	if (!c)
		return (*new_s);
	if (!(*new_s))
	{
		*new_s = malloc(2 * sizeof(char));
		if (!(*new_s))
			return (NULL);
		ft_memcpy(*new_s, &c, 1);
		(*new_s)[1] = '\0';
		return (*new_s);
	}
	tmp = *new_s;
	s_len = ft_strlen(*new_s);
	*new_s = malloc((s_len + 2) * sizeof (char));
	if (!(*new_s))
		return (NULL);
	ft_memcpy(*new_s, tmp, ft_strlen(tmp));
	(*new_s)[s_len] = c;
	(*new_s)[s_len + 1] = '\0';
	if (to_free == 1 && tmp)
		free(tmp);
	return (*new_s);
}
