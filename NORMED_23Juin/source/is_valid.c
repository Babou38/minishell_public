/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:54:41 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/18 15:54:42 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_empty_pipe(const char *str)
{
	int		i;
	bool	empty;

	i = 0;
	empty = false;
	while (str[i] && empty == false)
	{
		while (str[i] && (str[i] != '|' || (str[i] == '|'
					&& is_in_quotes(str, i))))
			i++;
		if (!str[i])
			return (false);
		if (str[i + 1] == '|')
			return (true);
		i++;
		empty = true;
		while (str[i] && (str[i] != '|' || (str[i] == '|'
					&& is_in_quotes(str, i))))
		{
			if ((!ft_strchr("|<>", str[i]) && !ft_isspace(str[i])))
				empty = false;
			i++;
		}
	}
	return (empty);
}

bool	is_ended_by_pipe(const char *str)
{
	int		i;
	int		j;
	bool	pipe;

	i = 0;
	j = -1;
	pipe = true;
	while (str[i])
	{
		if (str[i] == '|' && !is_in_quotes(str, i))
			j = i + 1;
		i++;
	}
	while (j > -1 && str[j] && pipe == true)
	{
		if (!ft_isspace(str[j]))
			pipe = false;
		j++;
	}
	if (j == -1)
		pipe = false;
	return (pipe);
}

bool	is_starting_by_pipe(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '|')
	{
		if (str[i] != '|' && !ft_isspace(str[i]))
			return (false);
		i++;
	}
	if (!str[i])
		return (false);
	return (true);
}

bool	is_invalid_char_in_quote(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((ft_strchr("~`#&*()\\[]{};!", str[i]) && !is_in_quotes(str, i))
			|| (str[i] == '?' && !is_in_quotes(str, i)
				&& i > 0 && str[i - 1] != '$'))
			return (true);
		i++;
	}
	return (false);
}

bool	is_command_valid(t_shell *shell, const char *message)
{
	if (are_quotes_closed(message))
		ft_putstr_fd(ERR_QUOTE_NOT_CLOSED, 2);
	if (is_invalid_char_in_quote(message))
		ft_putstr_fd(ERR_SPECIAL_CHAR, 2);
	else if (is_starting_by_pipe(message))
		ft_putstr_fd(ERR_PIPE_START, 2);
	else if (is_ended_by_pipe(message))
		ft_putstr_fd(ERR_PIPE_END, 2);
	else if (is_empty_pipe(message))
		ft_putstr_fd(ERR_EMPTY_PIPE, 2);
	else if (!is_redirection_valid(message))
		ft_putstr_fd(ERR_INVALID_REDIRECTION, 2);
	else
		return (true);
	ft_free((char *)message);
	shell->exitcode = 2;
	return (false);
}
//tous ce qui est avant a ete rajouté
/////////////////////////////////////////////////////////
