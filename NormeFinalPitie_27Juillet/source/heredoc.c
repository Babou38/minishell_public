/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 12:15:50 by babou             #+#    #+#             */
/*   Updated: 2024/06/10 11:37:38 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*read_heredoc(t_shell *shell, char *delimiter)
{
	char	*line;
	char	*heredoc_content;

	(void)shell;
	heredoc_content = ft_strdup("");
	while (42)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		else
			heredoc_content = ft_strjoin_f(heredoc_content, line, true);
		heredoc_content = ft_strjoin_f(heredoc_content, "\n", true);
		free(line);
	}
	return (heredoc_content);
}

void	handle_heredoc(t_shell *shell, char *delimiter)
{
	int		fd;
	char	*content;
	ssize_t	written;
	char	*counter_str;

	content = read_heredoc(shell, delimiter);
	counter_str = ft_itoa(shell->heredoc_counter++);
	if (shell->heredoc_name)
		free(shell->heredoc_name);
	shell->heredoc_name = ft_strjoin("heredoc_temp_", counter_str);
	free(counter_str);
	fd = open(shell->heredoc_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error(shell, 1);
	if (content)
	{
		written = write(fd, content, ft_strlen(content));
		if (written < 0)
			perror("write");
		free(content);
	}
	close(fd);
}

static int	get_del_length(char *str, char quote_char)
{
	int		length;
	char	*end;

	if (quote_char)
	{
		length = ft_strlen(str);
		end = ft_strchr(str, quote_char);
		if (end)
			length = end - str;
	}
	else
	{
		length = ft_get_delimiter(str);
		while (length > 0 && str[length - 1] == ' ')
			length--;
	}
	return (length);
}

static char	find_delim_quote_char(t_shell *shell, char **str)
{
	char	quote_char;

	quote_char = '\0';
	if (**str == '\'' || **str == '"')
	{
		quote_char = **str;
		(*str)++;
		shell->heredoc_quoted = true;
	}
	else
		shell->heredoc_quoted = false;
	return (quote_char);
}

char	*extract_delimiter(t_shell *shell, char *heredoc_pos, size_t length)
{
	char	*delimiter;
	char	quote_char;

	if (!*heredoc_pos)
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
	while (*heredoc_pos == ' ')
		heredoc_pos++;
	quote_char = find_delim_quote_char(shell, &heredoc_pos);
	length = get_del_length(heredoc_pos, quote_char);
	if (length == 0)
	{
		printf("bash: syntax error near unexpected token `<<'\n");
		return (NULL);
	}
	delimiter = malloc(length + 1);
	if (!delimiter)
		return (NULL);
	ft_strlcpy(delimiter, heredoc_pos, length + 1);
	return (delimiter);
}
