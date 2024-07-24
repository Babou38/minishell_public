/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:52:20 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/24 12:52:21 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*extract_var_name(const char **dollar_ptr)
{
	const char	*end;
	char		*var_name;

	end = *dollar_ptr;
	while (*end && (ft_isalnum(*end) || *end == '_'))
		end++;
	var_name = ft_strndup(*dollar_ptr, end - *dollar_ptr);
	*dollar_ptr = end;
	return (var_name);
}

static char	*join_part(const char *start, const char *end, char *result)
{
	char	*before;
	char	*temp;

	before = ft_strndup(start, end - start);
	temp = ft_strjoin(result, before);
	free(result);
	free(before);
	return (temp);
}

static char	*expand_single_variable(t_shell *shell, const char **start,
const char *dollar, char *result)
{
	char	*temp;
	char	*var_name;
	char	*var_value;

	result = join_part(*start, dollar, result);
	dollar++;
	var_name = extract_var_name(&dollar);
	var_value = get_env_value(shell, var_name);
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_name);
	*start = dollar;
	return (temp);
}

char	*expand_variables(t_shell *shell, const char *input)
{
	char		*result;
	const char	*start = input;
	const char	*dollar;
	char		*temp;
	char		*remaining;

	result = ft_strdup("");
	dollar = ft_strchr(start, '$');
	while (dollar != NULL)
	{
		temp = expand_single_variable(shell, &start, dollar, result);
		result = temp;
		dollar = ft_strchr(start, '$');
	}
	remaining = ft_strdup(start);
	temp = ft_strjoin(result, remaining);
	free(result);
	free(remaining);
	return (temp);
}
