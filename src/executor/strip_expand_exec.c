// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   strip_expand_exec.c                                :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/12/07 16:38:28 by jotong            #+#    #+#             */
// /*   Updated: 2025/12/09 15:21:37 by ksng             ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"
// #include <dirent.h>
// #include "libft.h"

// static void	init_quote_state(t_quote_state *state)
// {
// 	state->in_single = 0;
// 	state->in_double = 0;
// 	state->flag = 0;
// }

// /* ************************************************************************** */
// /*                           VARIABLE EXPANSION                               */
// /* ************************************************************************** */

// static size_t	get_var_len(char *s, int start)
// {
// 	size_t	len;

// 	len = 0;
// 	while (s[start + len] && (ft_isalnum(s[start + len])
// 		|| s[start + len] == '_'))
// 		len++;
// 	return (len);
// }

// static char	*get_env_value(char *var_name, char **env)
// {
// 	int		i;
// 	size_t	len;

// 	if (!var_name || !env)
// 		return (NULL);
// 	len = ft_strlen(var_name);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
// 			return (env[i] + len + 1);
// 		i++;
// 	}
// 	return (NULL);
// }

// static char	*get_home_dir(char **env)
// {
// 	char	*home;

// 	home = get_env_value("HOME", env);
// 	if (!home)
// 		home = "/tmp";
// 	return (home);
// }

// /* ************************************************************************** */
// /*                           WILDCARD EXPANSION                               */
// /* ************************************************************************** */

// static int	match_pattern_recursive(char *str, char *pattern)
// {
// 	if (!*pattern)
// 		return (!*str);
// 	if (*pattern == '*')
// 	{
// 		while (*pattern == '*')
// 			pattern++;
// 		if (!*pattern)
// 			return (1);
// 		while (*str)
// 		{
// 			if (match_pattern_recursive(str, pattern))
// 				return (1);
// 			str++;
// 		}
// 		return (0);
// 	}
// 	if (*str && (*str == *pattern || *pattern == '?'))
// 		return (match_pattern_recursive(str + 1, pattern + 1));
// 	return (0);
// }

// int	match_pattern(char *str, char *pattern)
// {
// 	if (ft_strcmp(str, ".") == 0 || ft_strcmp(str, "..") == 0)
// 		return (0);
// 	if (str[0] == '.' && pattern[0] != '.')
// 		return (0);
// 	return (match_pattern_recursive(str, pattern));
// }

// static int	should_skip_entry(char *name, char *pattern)
// {
// 	if (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0)
// 		return (1);
// 	if (name[0] == '.' && pattern[0] != '.')
// 		return (1);
// 	return (0);
// }

// static char **get_matching_files(char *pattern)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			**matches;
// 	int				count;

// 	dir = opendir(".");
// 	if (!dir)
// 		return (NULL);
// 	matches = malloc(sizeof(char *) * 1024);
// 	count = 0;
// 	entry = readdir(dir);
// 	while (entry && count < 1023)
// 	{
// 		if (should_skip_entry(entry->d_name, pattern))
// 		{
// 			entry = readdir(dir);
// 			continue;
// 		}
// 		if (match_pattern(entry->d_name, pattern))
// 			matches[count++] = ft_strdup(entry->d_name);
// 		entry = readdir(dir);
// 	}
// 	matches[count] = NULL;
// 	closedir(dir);
// 	if (count == 0)
// 	{
// 		free(matches);
// 		return (NULL);
// 	}
// 	return (matches);
// }

// static char	*join_matches(char **matches)
// {
// 	char	*result;
// 	char	*tmp;
// 	int		i;

// 	if (!matches || !matches[0])
// 		return (NULL);
// 	result = ft_strdup(matches[0]);
// 	i = 1;
// 	while (matches[i])
// 	{
// 		tmp = result;
// 		result = ft_strjoin(result, " ");
// 		free(tmp);
// 		tmp = result;
// 		result = ft_strjoin(result, matches[i]);
// 		free(tmp);
// 		i++;
// 	}
// 	return (result);
// }

// static void	free_matches(char **matches)
// {
// 	int	i;

// 	if (!matches)
// 		return ;
// 	i = 0;
// 	while (matches[i])
// 	{
// 		free(matches[i]);
// 		i++;
// 	}
// 	free(matches);
// }

// /* ************************************************************************** */
// /*                      WILDCARD PATTERN DETECTION                            */
// /* ************************************************************************** */

// static int	contains_wildcard(char *s, char *end)
// {
// 	while (s < end)
// 	{
// 		if (*s == '*' || *s == '?')
// 			return (1);
// 		s++;
// 	}
// 	return (0);
// }

// static int	has_wildcard_ahead(char *s, t_quote_state *state)
// {
// 	char	*tmp;

// 	tmp = s;
// 	while (*tmp && *tmp != ' ' && *tmp != '\t' && *tmp != '"' && *tmp != '\'')
// 	{
// 		if ((*tmp == '*' || *tmp == '?') && !state->in_single && !state->in_double)
// 			return (1);
// 		tmp++;
// 	}
// 	return (0);
// }

// /* ************************************************************************** */
// /*                           LENGTH CALCULATION                               */
// /* ************************************************************************** */

// static size_t	calc_tilde_len(char *s, char **env)
// {
// 	char	*home;

// 	(void)s;
// 	home = get_home_dir(env);
// 	return (ft_strlen(home));
// }

// static size_t	calc_var_len(char *s, char **env)
// {
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;
// 	size_t	result;

// 	var_len = get_var_len(s, 0);
// 	if (var_len == 0)
// 		return (1);
// 	var_name = ft_substr(s, 0, var_len);
// 	if (!var_name)
// 		return (0);
// 	var_value = get_env_value(var_name, env);
// 	result = var_value ? ft_strlen(var_value) : 0;
// 	free(var_name);
// 	return (result);
// }

// static size_t	calc_wildcard_len(char **s)
// {
// 	char	*pattern_start;
// 	char	*pattern_end;
// 	char	*pattern;
// 	char	**matches;
// 	char	*joined;
// 	size_t	len;

// 	pattern_start = *s;
// 	while (**s && **s != ' ' && **s != '\t' && **s != '"' && **s != '\'')
// 		(*s)++;
// 	pattern_end = *s;
// 	if (!contains_wildcard(pattern_start, pattern_end))
// 		return (pattern_end - pattern_start);
// 	pattern = ft_substr(pattern_start, 0, pattern_end - pattern_start);
// 	matches = get_matching_files(pattern);
// 	free(pattern);
// 	if (!matches)
// 		return (pattern_end - pattern_start);
// 	joined = join_matches(matches);
// 	len = joined ? ft_strlen(joined) : 0;
// 	free(joined);
// 	free_matches(matches);
// 	return (len);
// }

// static size_t	calculate_expanded_len(char *s, char **env)
// {
// 	size_t			len;
// 	t_quote_state	state;
// 	char			*start;

// 	start = s;
// 	len = 0;
// 	init_quote_state(&state);
// 	while (*s)
// 	{
// 		if (*s == '\'' && !state.in_double)
// 		{
// 			state.in_single = !state.in_single;
// 			s++;
// 		}
// 		else if (*s == '"' && !state.in_single)
// 		{
// 			state.in_double = !state.in_double;
// 			s++;
// 		}
// 		else if (*s == '~' && !state.in_single && !state.in_double
// 			&& (s == start || *(s - 1) == ' '))
// 		{
// 			s++;
// 			len += calc_tilde_len(s, env);
// 		}
// 		else if (*s == '$' && !state.in_single && *(s + 1))
// 		{
// 			s++;
// 			len += calc_var_len(s, env);
// 			s += get_var_len(s, 0);
// 		}
// 		else if (!state.in_single && !state.in_double &&
// 			(s == start || *(s - 1) == ' ' || *(s - 1) == '\t') &&
// 			has_wildcard_ahead(s, &state))
// 		{
// 			len += calc_wildcard_len(&s);
// 		}
// 		else
// 		{
// 			len++;
// 			s++;
// 		}
// 	}
// 	return (len);
// }

// /* ************************************************************************** */
// /*                           EXPANSION EXECUTION                              */
// /* ************************************************************************** */

// static int	expand_tilde(char **s, char **out, int *i, char **env)
// {
// 	char	*home;

// 	home = get_home_dir(env);
// 	ft_strlcpy(*out + *i, home, ft_strlen(home) + 1);
// 	*i += ft_strlen(home);
// 	(*s)++;
// 	return (1);
// }

// static int	expand_variable(char **s, char **out, int *i, char **env)
// {
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;

// 	(*s)++;
// 	var_len = get_var_len(*s, 0);
// 	if (var_len == 0)
// 	{
// 		(*out)[(*i)++] = '$';
// 		return (1);
// 	}
// 	var_name = ft_substr(*s, 0, var_len);
// 	if (!var_name)
// 		return (0);
// 	var_value = get_env_value(var_name, env);
// 	if (var_value != NULL)
// 	{
// 		ft_strlcpy(*out + *i, var_value, ft_strlen(var_value) + 1);
// 		*i += ft_strlen(var_value);
// 	}
// 	free(var_name);
// 	*s += var_len;
// 	return (1);
// }

// static int	expand_wildcard(char **s, char **out, int *i)
// {
// 	char	*pattern_start;
// 	char	*pattern_end;
// 	char	*pattern;
// 	char	**matches;
// 	char	*joined;

// 	pattern_start = *s;
// 	while (**s && **s != ' ' && **s != '\t' && **s != '"' && **s != '\'')
// 		(*s)++;
// 	pattern_end = *s;
// 	if (!contains_wildcard(pattern_start, pattern_end))
// 	{
// 		while (pattern_start < pattern_end)
// 			(*out)[(*i)++] = *pattern_start++;
// 		return (1);
// 	}
// 	pattern = ft_substr(pattern_start, 0, pattern_end - pattern_start);
// 	matches = get_matching_files(pattern);
// 	if (!matches)
// 	{
// 		ft_strlcpy(*out + *i, pattern, ft_strlen(pattern) + 1);
// 		*i += ft_strlen(pattern);
// 		free(pattern);
// 		return (1);
// 	}
// 	joined = join_matches(matches);
// 	ft_strlcpy(*out + *i, joined, ft_strlen(joined) + 1);
// 	*i += ft_strlen(joined);
// 	free(pattern);
// 	free(joined);
// 	free_matches(matches);
// 	return (1);
// }

// static int	copy_with_expansion(char *s, char *out, char **env)
// {
// 	int				i;
// 	t_quote_state	state;
// 	char			*start;

// 	i = 0;
// 	start = s;
// 	init_quote_state(&state);
// 	while (*s)
// 	{
// 		if (*s == '\'' && !state.in_double)
// 		{
// 			state.in_single = !state.in_single;
// 			s++;
// 		}
// 		else if (*s == '"' && !state.in_single)
// 		{
// 			state.in_double = !state.in_double;
// 			s++;
// 		}
// 		else if (*s == '~' && !state.in_single && !state.in_double
// 			&& (s == start || *(s - 1) == ' '))
// 		{
// 			if (!expand_tilde(&s, &out, &i, env))
// 				return (0);
// 		}
// 		else if (*s == '$' && !state.in_single && *(s + 1))
// 		{
// 			if (!expand_variable(&s, &out, &i, env))
// 				return (0);
// 		}
// 		else if (!state.in_single && !state.in_double &&
// 			(s == start || *(s - 1) == ' ' || *(s - 1) == '\t') &&
// 			has_wildcard_ahead(s, &state))
// 		{
// 			if (!expand_wildcard(&s, &out, &i))
// 				return (0);
// 		}
// 		else
// 		{
// 			out[i++] = *s++;
// 		}
// 	}
// 	out[i] = '\0';
// 	return (1);
// }

// char	*expand_and_replace(char **s, t_shell *shell)
// {
// 	char	*out;
// 	size_t	len;
// 	char	*original;

// 	if (!s || !*s)
// 		return (NULL);
// 	original = *s;
// 	len = calculate_expanded_len(original, shell->envp);
// 	out = malloc(len + 1);
// 	if (!out)
// 		return (NULL);
// 	if (!copy_with_expansion(original, out, shell->envp))
// 	{
// 		free(out);
// 		return (NULL);
// 	}
// 	free(original);
// 	*s = out;
// 	return (out);
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:38:28 by jotong            #+#    #+#             */
/*   Updated: 2025/12/09 14:38:13 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include "libft.h"

static void	init_quote_state(t_quote_state *state)
{
	state->in_single = 0;
	state->in_double = 0;
	state->flag = 0;
}

/* ************************************************************************** */
/*                           HELPER FUNCTIONS                                 */
/* ************************************************************************** */

static size_t	get_var_len(char *s, int start)
{
	size_t	len;

	len = 0;
	while (s[start + len] && (ft_isalnum(s[start + len])
		|| s[start + len] == '_'))
		len++;
	return (len);
}

static char	*get_env_value(char *var_name, char **env)
{
	int		i;
	size_t	len;

	if (!var_name || !env)
		return (NULL);
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*get_home_dir(char **env)
{
	char	*home;

	home = get_env_value("HOME", env);
	if (!home)
		home = "/tmp";
	return (home);
}

/* ************************************************************************** */
/*                           WILDCARD MATCHING                                */
/* ************************************************************************** */

static int	match_pattern_recursive(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern_recursive(str, pattern))
				return (1);
			str++;
		}
		return (0);
	}
	if (*str && (*str == *pattern || *pattern == '?'))
		return (match_pattern_recursive(str + 1, pattern + 1));
	return (0);
}

int	match_pattern(char *str, char *pattern)
{
	if (ft_strcmp(str, ".") == 0 || ft_strcmp(str, "..") == 0)
		return (0);
	if (str[0] == '.' && pattern[0] != '.')
		return (0);
	return (match_pattern_recursive(str, pattern));
}

static char **get_matching_files(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 1024);
	count = 0;
	entry = readdir(dir);
	while (entry && count < 1023)
	{
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
		{
			entry = readdir(dir);
			continue;
		}
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue;
		}
		if (match_pattern(entry->d_name, pattern))
			matches[count++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[count] = NULL;
	closedir(dir);
	if (count == 0)
	{
		free(matches);
		return (NULL);
	}
	return (matches);
}

static void	free_matches(char **matches)
{
	int	i;

	if (!matches)
		return ;
	i = 0;
	while (matches[i])
	{
		free(matches[i]);
		i++;
	}
	free(matches);
}

/* ************************************************************************** */
/*                   STEP 1: CALCULATE LENGTH WITH EXPANSIONS                 */
/* ************************************************************************** */

static size_t	calc_var_expansion_len(char *s, char **env)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	size_t	result;

	var_len = get_var_len(s, 0);
	if (var_len == 0)
		return (1);
	var_name = ft_substr(s, 0, var_len);
	if (!var_name)
		return (0);
	var_value = get_env_value(var_name, env);
	result = var_value ? ft_strlen(var_value) : 0;
	free(var_name);
	return (result);
}

static size_t	calculate_len_step1(char *s, char **env)
{
	size_t			len;
	t_quote_state	state;
	char			*start;

	start = s;
	len = 0;
	init_quote_state(&state);
	while (*s)
	{
		if (*s == '\'' && !state.in_double)
		{
			state.in_single = !state.in_single;
			s++;
		}
		else if (*s == '"' && !state.in_single)
		{
			state.in_double = !state.in_double;
			s++;
		}
		else if (*s == '~' && !state.in_single && !state.in_double
			&& (s == start || *(s - 1) == ' '))
		{
			len += ft_strlen(get_home_dir(env));
			s++;
		}
		else if (*s == '$' && !state.in_single && *(s + 1))
		{
			s++;
			len += calc_var_expansion_len(s, env);
			s += get_var_len(s, 0);
		}
		else
		{
			len++;
			s++;
		}
	}
	return (len);
}

/* ************************************************************************** */
/*        STEP 2: EXPAND VARIABLES/TILDE AND STRIP QUOTES WITH TRACKING      */
/* ************************************************************************** */

typedef struct s_expand_state
{
	char			*out;
	char			*quote_map;
	int				i;
	t_quote_state	state;
	char			*start;
}	t_expand_state;

static int	expand_tilde_step2(char **s, t_expand_state *es, char **env)
{
	char    *home;
    size_t  home_len;
    size_t  j;
    int     quoted;

    home = get_home_dir(env);
    home_len = ft_strlen(home);
    quoted = (es->state.in_single || es->state.in_double) ? 1 : 0;
    j = 0;
    while (j < home_len)
    {
        es->out[es->i] = home[j];
        es->quote_map[es->i] = quoted;
        es->i++;
        j++;
    }
    (*s)++;
    return (1);
}

static int	expand_var_step2(char **s, t_expand_state *es, char **env)
{
	size_t  var_len;
    char    *var_name;
    char    *var_value;
    size_t  j;
    int     quoted;

    (*s)++;
    var_len = get_var_len(*s, 0);
    if (var_len == 0)
    {
        /* put literal $ back */
        es->out[es->i] = '$';
        es->quote_map[es->i] = (es->state.in_single || es->state.in_double) ? 1 : 0;
        es->i++;
        return (1);
    }
    var_name = ft_substr(*s, 0, var_len);
    if (!var_name)
        return (0);
    var_value = get_env_value(var_name, env);
    quoted = (es->state.in_single || es->state.in_double) ? 1 : 0;
    if (var_value)
    {
        j = 0;
        while (var_value[j])
        {
            es->out[es->i] = var_value[j];
            es->quote_map[es->i] = quoted;
            es->i++;
            j++;
        }
    }
    free(var_name);
    *s += var_len;
    return (1);
}

static char	*expand_and_strip_quotes(char *s, char **env, char **quote_map_out)
{
	t_expand_state	es;
	size_t			len;

	len = calculate_len_step1(s, env);
	es.out = malloc(len + 1);
	es.quote_map = malloc(len + 1);
	if (!es.out || !es.quote_map)
	{
		free(es.out);
		free(es.quote_map);
		return (NULL);
	}
	es.i = 0;
	es.start = s;
	init_quote_state(&es.state);
	while (*s)
	{
		if (*s == '\'' && !es.state.in_double)
		{
			es.state.in_single = !es.state.in_single;
			s++;
		}
		else if (*s == '"' && !es.state.in_single)
		{
			es.state.in_double = !es.state.in_double;
			s++;
		}
		else if (*s == '~' && !es.state.in_single && !es.state.in_double
			&& (s == es.start || *(s - 1) == ' '))
		{
			if (!expand_tilde_step2(&s, &es, env))
			{
				free(es.out);
				free(es.quote_map);
				return (NULL);
			}
		}
		else if (*s == '$' && !es.state.in_single && *(s + 1))
		{
			if (!expand_var_step2(&s, &es, env))
			{
				free(es.out);
				free(es.quote_map);
				return (NULL);
			}
		}
		else
		{
			es.out[es.i] = *s;
			es.quote_map[es.i] = (es.state.in_single || es.state.in_double) ? 1 : 0;
			es.i++;
			s++;
		}
	}
	es.out[es.i] = '\0';
	es.quote_map[es.i] = '\0';
	*quote_map_out = es.quote_map;
	return (es.out);
}

/* ************************************************************************** */
/*              STEP 3: EXPAND WILDCARDS (ONLY UNQUOTED)                     */
/* ************************************************************************** */

static int	has_unquoted_wildcard(char *s, char *quote_map, int start, int end)
{
	int	i;

	i = start;
	while (i < end)
	{
		if ((s[i] == '*' || s[i] == '?') && quote_map[i] == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	find_word_end(char *s, int start)
{
	int	i;

	i = start;
	while (s[i] && s[i] != ' ' && s[i] != '\t')
		i++;
	return (i);
}

static char	*expand_wildcard_pattern(char *pattern)
{
	char	**matches;
	char	*result;
	char	*tmp;
	int		i;

	matches = get_matching_files(pattern);
	if (!matches)
		return (ft_strdup(pattern));
	result = ft_strdup(matches[0]);
	i = 1;
	while (matches[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		tmp = result;
		result = ft_strjoin(result, matches[i]);
		free(tmp);
		i++;
	}
	free_matches(matches);
	return (result);
}

static size_t	calc_wildcard_expanded_len(char *s, char *quote_map)
{
	int		i;
	int		word_start;
	int		word_end;
	char	*pattern;
	char	*expanded;
	size_t	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			len++;
			i++;
		}
		else
		{
			word_start = i;
			word_end = find_word_end(s, i);
			if (has_unquoted_wildcard(s, quote_map, word_start, word_end))
			{
				pattern = ft_substr(s, word_start, word_end - word_start);
				expanded = expand_wildcard_pattern(pattern);
				len += ft_strlen(expanded);
				free(pattern);
				free(expanded);
			}
			else
				len += word_end - word_start;
			i = word_end;
		}
	}
	return (len);
}

static char	*expand_wildcards(char *s, char *quote_map)
{
	char	*result;
	size_t	len;
	int		i;
	int		j;
	int		word_start;
	int		word_end;
	char	*pattern;
	char	*expanded;
	int		k;

	len = calc_wildcard_expanded_len(s, quote_map);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			result[j++] = s[i++];
		}
		else
		{
			word_start = i;
			word_end = find_word_end(s, i);
			if (has_unquoted_wildcard(s, quote_map, word_start, word_end))
			{
				pattern = ft_substr(s, word_start, word_end - word_start);
				expanded = expand_wildcard_pattern(pattern);
				k = 0;
				while (expanded[k])
					result[j++] = expanded[k++];
				free(pattern);
				free(expanded);
			}
			else
			{
				while (i < word_end)
					result[j++] = s[i++];
			}
			i = word_end;
		}
	}
	result[j] = '\0';
	return (result);
}

/* ************************************************************************** */
/*                           MAIN ENTRY POINT                                 */
/* ************************************************************************** */

char	*expand_and_replace(char **s, t_shell *shell)
{
	char	*step1_result;
	char	*quote_map;
	char	*step2_result;
	char	*original;

	if (!s || !*s)
		return (NULL);
	original = *s;
	step1_result = expand_and_strip_quotes(original, shell->envp, &quote_map);
	if (!step1_result)
		return (NULL);
	step2_result = expand_wildcards(step1_result, quote_map);
	free(step1_result);
	free(quote_map);
	if (!step2_result)
		return (NULL);
	free(original);
	*s = step2_result;
	return (step2_result);
}
