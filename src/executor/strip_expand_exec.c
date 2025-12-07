/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:32:06 by ksng              #+#    #+#             */
/*   Updated: 2025/12/07 15:46:51 by jotong           ###   ########.fr       */
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

// static void	update_quote_state(char c, t_quote_state *state)
// {
// 	if (c == '\'' && !state->in_double)
// 		state->in_single = !state->in_single;
// 	else if (c == '"' && !state->in_single)
// 		state->in_double = !state->in_double;
// }

/* ************************************************************************** */
/*                           VARIABLE EXPANSION                               */
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
/*                           WILDCARD EXPANSION                               */
/* ************************************************************************** */

int match_pattern(char *str, char *pattern)
{
    // R1: Termination
    if (!*pattern)
        return (!*str);
    
    // R2: Dotfile and Simple Character Matching
    // Check if both strings start with a dot, OR if neither starts with a dot.
    if (*str != *pattern && *pattern != '?')
    {
        // If the pattern starts with a literal char (or '?') AND it doesn't match
        // the current char in str, then fail, unless pattern is '*'.
        if (*pattern != '*')
            return (0);
    }

    // R2.1: CRITICAL Dotfile Enforcement (Must fail if pattern requires a dot but string doesn't)
    if (*pattern == '.') 
    {
        // If pattern starts with '.' but str does not, fail.
        if (*str != '.')
            return (0); 
    } 
    else if (*str == '.')
    {
        // If str starts with '.' but pattern does not, fail (unless pattern is '*').
        if (*pattern != '*')
            return (0);
    }
    
    // R3: Wildcard '*'
    if (*pattern == '*')
    {
        while (*pattern == '*')
            pattern++;
        if (!*pattern)
            return (1);
        
        while (*str)
        {
            // IMPORTANT: Recursively call match_pattern with the rest of the pattern (pattern)
            // against the rest of the string (str).
            if (match_pattern(str, pattern))
                return (1);
            str++;
        }
        return (0); 
    }
    
    // R4: Single Character Match ('?' or Literal Match)
    if (*str && (*str == *pattern || *pattern == '?'))
        return (match_pattern(str + 1, pattern + 1));
    
    return (0);
}

static char **get_matching_files(char *pattern)
{
    DIR             *dir;
    struct dirent   *entry;
    char            **matches;
    int             count;

    dir = opendir(".");
    if (!dir)
        return (NULL);
    matches = malloc(sizeof(char *) * 1024);
    count = 0;
    entry = readdir(dir);
    while (entry && count < 1023)
	{
		// Skip '.' and '..' unless the pattern is explicitly targeting them
		if ((ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0) &&
			(ft_strcmp(pattern, ".") != 0 && ft_strcmp(pattern, "..") != 0 && pattern[0] != '.'))
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

static char	*join_matches(char **matches)
{
	char	*result;
	char	*tmp;
	int		i;

	if (!matches || !matches[0])
		return (NULL);
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
	return (result);
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
/*                           LENGTH CALCULATION                               */
/* ************************************************************************** */

static size_t	calc_tilde_len(char *s, char **env)
{
	char	*home;

	(void)s;
	home = get_home_dir(env);
	return (ft_strlen(home));
}

static size_t	calc_var_len(char *s, char **env)
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

static size_t	calc_wildcard_len(char **s)
{
	char	*pattern_start;
	char	*pattern_end;
	char	*pattern;
	char	**matches;
	char	*joined;
	size_t	len;

	pattern_start = *s;
	while (**s && **s != ' ' && **s != '\t' && **s != '"' && **s != '\'')
		(*s)++;
	pattern_end = *s;
	pattern = ft_substr(pattern_start, 0, pattern_end - pattern_start);
	matches = get_matching_files(pattern);
	free(pattern);
	if (!matches)
		return (pattern_end - pattern_start);
	joined = join_matches(matches);
	len = joined ? ft_strlen(joined) : 0;
	free(joined);
	free_matches(matches);
	return (len);
}

static size_t	calculate_expanded_len(char *s, char **env)
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
			s++;
			len += calc_tilde_len(s, env);
		}
		else if (*s == '$' && !state.in_single && *(s + 1))
		{
			s++;
			len += calc_var_len(s, env);
			s += get_var_len(s, 0);
		}
		else if (*s == '*' && !state.in_single && !state.in_double)
		{
			len += calc_wildcard_len(&s);
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
/*                           EXPANSION EXECUTION                              */
/* ************************************************************************** */

static int	expand_tilde(char **s, char **out, int *i, char **env)
{
	char	*home;

	home = get_home_dir(env);
	ft_strlcpy(*out + *i, home, ft_strlen(home) + 1);
	*i += ft_strlen(home);
	(*s)++;
	return (1);
}

static int	expand_variable(char **s, char **out, int *i, char **env)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;

	(*s)++;
	var_len = get_var_len(*s, 0);
	if (var_len == 0)
	{
		(*out)[(*i)++] = '$';
		return (1);
	}
	var_name = ft_substr(*s, 0, var_len);
	if (!var_name)
		return (0);
	var_value = get_env_value(var_name, env);
	if (var_value != NULL)
	{
		ft_strlcpy(*out + *i, var_value, ft_strlen(var_value) + 1);
		*i += ft_strlen(var_value);
	}
	free(var_name);
	*s += var_len;
	return (1);
}

static int	expand_wildcard(char **s, char **out, int *i)
{
	char	*pattern_start;
	char	*pattern_end;
	char	*pattern;
	char	**matches;
	char	*joined;

	pattern_start = *s;
	while (**s && **s != ' ' && **s != '\t' && **s != '"' && **s != '\'')
		(*s)++;
	pattern_end = *s;
	pattern = ft_substr(pattern_start, 0, pattern_end - pattern_start);
	matches = get_matching_files(pattern);
	if (!matches)
	{
		ft_strlcpy(*out + *i, pattern, ft_strlen(pattern) + 1);
		*i += ft_strlen(pattern);
		free(pattern);
		return (1);
	}
	joined = join_matches(matches);
	ft_strlcpy(*out + *i, joined, ft_strlen(joined) + 1);
	*i += ft_strlen(joined);
	free(pattern);
	free(joined);
	free_matches(matches);
	return (1);
}

static int	copy_with_expansion(char *s, char *out, char **env)
{
	int				i;
	t_quote_state	state;
	char			*start;

	i = 0;
	start = s;
	init_quote_state(&state);
	while (*s)
	{
		// Only strip quotes if they're actual delimiters, not literals
		if (*s == '\'' && !state.in_double)
		{
			state.in_single = !state.in_single;
			s++;  // Skip the quote (it's a delimiter)
		}
		else if (*s == '"' && !state.in_single)
		{
			state.in_double = !state.in_double;
			s++;  // Skip the quote (it's a delimiter)
		}
		else if (*s == '~' && !state.in_single && !state.in_double
			&& (s == start || *(s - 1) == ' '))
		{
			if (!expand_tilde(&s, &out, &i, env))
				return (0);
		}
		else if (*s == '$' && !state.in_single && *(s + 1))
		{
			if (!expand_variable(&s, &out, &i, env))
				return (0);
			// if (*s == '*')
			// {
			// 	if (!expand_wildcard(&s, &out, &i))
			// 		return (0);
			// }
		}
		else if (*s == '*' && !state.in_single && !state.in_double)
		{
			if (!expand_wildcard(&s, &out, &i))
				return (0);
		}
		else
		{
			// Copy everything else (including quotes that are literals)
			out[i++] = *s++;
		}
	}
	out[i] = '\0';
	return (1);
}

char	*expand_and_replace(char **s, t_shell *shell)
{
	char	*out;
	size_t	len;
	char	*original;

	if (!s || !*s)
		return (NULL);

	original = *s;
	len = calculate_expanded_len(original, shell->envp);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	if (!copy_with_expansion(original, out, shell->envp))
	{
		free(out);
		return (NULL);
	}
	
	free(original);  // Free the original
	*s = out;        // Update caller's pointer
	return (out);
}
