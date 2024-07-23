/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:51:23 by bchapuis          #+#    #+#             */
/*   Updated: 2024/06/10 11:35:28 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*path_string(t_shell *shell)
{
	char	*path;
	char	*path_end;
	char	*pwd;
	char	*colored_prompt;
	int		err;

	pwd = ft_get_cwd(shell, &err);
	colored_prompt = "\001\033[1;34m\002minishell(\001\033[1;31m\002";
	path = ft_strjoin_f(colored_prompt, pwd, false);
	free(pwd);
	path_end = ft_strjoin_f(path, "\001\033[1;34m\002)> \001\033[0m\002",
			false);
	free(path);
	return (path_end);
}

char	*handle_incomplete_pipe(char *input)
{
	char	*temp_input;

	temp_input = NULL;
	while (!temp_input || !*temp_input)
	{
		if (temp_input)
			free(temp_input);
		temp_input = readline("> ");
		if (temp_input == NULL)
		{
			free(input);
			return (NULL);
		}
	}
	input = strjoin_and_free(input, temp_input);
	free(temp_input);
	if (!input)
		return (NULL);
	return (input);
}

bool	quotes_closed(const char *input)
{
	bool	single_quote_open;
	bool	double_quote_open;

	single_quote_open = false;
	double_quote_open = false;
	while (*input)
	{
		if (*input == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		else if (*input == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		input++;
	}
	return (!single_quote_open && !double_quote_open);
}

bool	valid_quotes(char *input)
{
	if (!quotes_closed(input))
	{
		printferr("Error: Unclosed quotes\n");
		free(input);
		return (false);
	}
	return (true);
}

char	*ft_read_input(t_shell *shell)
{
	char	*path;
	char	*str;
	char	*input;

	path = path_string(shell);
	str = readline(path);
	free(path);
	if (str != NULL && str[0])
		add_history(str);
	if (str == NULL)
		return (NULL);
	if (handle_isspace(str))
	{
		free(str);
		return (ft_strdup("\0"));
	}
	input = ft_strdup(str);
	free(str);
	if (!valid_quotes(input))
		return (NULL);
	input = expand_input(shell, input);
	input = search_heredoc(shell, input, 0);
	return (input);
}
