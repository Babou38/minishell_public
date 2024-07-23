/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 15:09:09 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/22 15:09:10 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_dollar_value(t_shell *shell, char *str, int begin, int end)
{
	char	*to_search;
	char	*var_value;
	int		k;
	int		j;

	k = begin + 1;
	j = 0;
	to_search = malloc(sizeof(char) * (end - begin + 2));
	if (!to_search)
		safe_exit(shell, NULL, NONE, 2);
	while (begin++ <= end)
		to_search[j++] = str[k++];
	to_search[j - 1] = '\0';
	if (ft_strcmp(to_search, "?") == 0)
		var_value = ft_itoa(shell->exitcode);
	else
		var_value = ft_getenv(shell, to_search);
	free(to_search);
	return (var_value);
}

int	find_malloc_size(t_shell *shell, char *str, int begin, int end)
{
	int		var_size;
	int		to_alloc;

	var_size = 0;
	to_alloc = 0;
	while (str[begin])
	{
		if (contains_global_var(str, &begin, &end, begin))
		{
			if (is_in_quotes(str, begin) != 1 && str[begin + 1] != '\0'
				&& str[begin + 1] != '\'' && str[begin + 1] != '"')
			{
				var_size += end - begin + 1;
				to_alloc += extract_dollar_size(shell, str, begin, end);
			}
			if (begin == end)
				begin = end + 1;
			else
				begin = end;
		}
		else
			break ;
	}
	return (to_alloc - var_size);
}

char	*extracting_doll(t_shell *shell, char *str, char *res, t_nb *pos)
{
	int		begin;
	int		end;
	int		k;
	char	*value;

	end = 0;
	begin = 0;
	k = 0;
	contains_global_var(str, &begin, &end, pos->i);
	value = extract_dollar_value(shell, str, begin, end);
	while (value && value[k])
	{
		res[(pos->j)++] = value[k];
		k++;
	}
	pos->i = end + 1;
	free(value);
	return (res);
}

char	*replace_dollar_value(t_shell *shell, char *str, char *res)
{
	t_nb	pos;

	pos.i = 0;
	pos.j = 0;
	while (str[pos.i])
	{
		if (str[pos.i] == '$' && (is_in_quotes(str, pos.i) != 1)
			&& str[pos.i + 1] != '\0'
			&& str[pos.i + 1] != '"' && str[pos.i + 1] != '\''
			&& str[pos.i + 1] != ' ')
			res = extracting_doll(shell, str, res, &pos);
		else
		{
			res[pos.j] = str[pos.i];
			pos.j += 1;
			pos.i += 1;
			res[pos.j] = '\0';
		}
	}
	res[pos.j] = '\0';
	return (res);
}

char	*expand_input(t_shell *shell, char *str)
{
	char	*res;
	int		base_size;
	int		to_alloc;

	res = NULL;
	base_size = ft_strlen(str);
	to_alloc = find_malloc_size(shell, str, 0, 0);
	res = malloc(sizeof(char) * (base_size + to_alloc + 1));
	res = replace_dollar_value(shell, str, res);
	free(str);
	return (res);
}
