/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 17:58:50 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/18 17:58:51 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_shell *shell, char *var_name)
{
	t_env_list	*current;

	current = shell->list;
	while (current != NULL)
	{
		if (ft_strcmp(current->var_name, var_name) == 0)
		{
			if (current->value)
				return (current->value);
			else
				return ("");
		}
		current = current->next;
	}
	return ("");
}

bool	check_if_open(char **input, char quote, bool *var, bool *truth)
{
	if (**input == quote && !(*var))
	{
		*truth = !(*truth);
		(*input) += 1;
		return (true);
	}
	return (false);
}

char	*handle_quotes_doll(t_shell *shell, char *input, char *res_ptr)
{
	char	*var_start;
	char	*var_name;
	char	*var_value;

	var_start = input + 1;
	while (ft_isalnum(*var_start) || *var_start == '_')
		var_start++;
	var_name = ft_strndup(input + 1, var_start - (input + 1));
	var_value = get_env_value(shell, var_name);
	free(var_name);
	if (var_value)
	{
		ft_strcpy(res_ptr, var_value);
		res_ptr += ft_strlen(var_value);
	}
	input = var_start;
	return (input);
}

char	*search_heredoc(t_shell *shell, char *input, size_t i)
{
	char	*delimiter;
	char	*heredoc_pos;

	delimiter = NULL;
	while (input && input[i])
	{
		heredoc_pos = ft_strnstr_i(input, "<<", ft_strlen(input), &i);
		if (!is_in_quotes(input, i) && heredoc_pos)
		{
			heredoc_pos += 2;
			delimiter = extract_delimiter(shell, heredoc_pos, 0);
			printf("delim is : (%s)\n", delimiter); // to remove
			if (!delimiter)
				safe_exit(shell, input, STR, 1);
			handle_heredoc(shell, delimiter);
			free(delimiter);
			input = handle_quotes(shell, input, false, false);
			i++;
		}
		else
			break ;
	}
	return (input);
}

char	*handle_quotes(t_shell *shell, char *input, bool s_is_open,
	bool d_is_open)
{
	char	*result;
	char	*res_ptr;
	char	*to_free;

	result = malloc(ft_strlen(input) + 1);
	res_ptr = result;
	to_free = input;
	while (*input)
	{
		if (check_if_open(&input, '\'', &d_is_open, &s_is_open)
			|| check_if_open(&input, '\"', &s_is_open, &d_is_open))
			continue ;
		if (d_is_open)
		{
			if (*input == '$')
				input = handle_quotes_doll(shell, input, res_ptr);
			else
				*res_ptr++ = *input++;
		}
		else
			*res_ptr++ = *input++;
	}
	*res_ptr = '\0';
	free(to_free);
	return (result);
}
