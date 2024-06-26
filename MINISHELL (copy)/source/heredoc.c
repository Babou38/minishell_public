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

char *get_env_value(t_shell *shell, char *var_name)
{
	t_env_list *current;

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

// char *expand_variables(t_shell *shell, const char *input)    // pas utile ? 
// {
// 	char *result;
// 	const char *start;
// 	const char *dollar;
// 	char *before;
// 	char *temp;
// 	const char *end;
// 	char *var_name;
// 	char *var_value;
// 	char *remaining;

// 	result = ft_strdup("");
// 	start = input;
// 	while ((dollar = ft_strchr(start, '$')) != NULL)
// 	{
// 		before = ft_strndup(start, dollar - start); //to check
// 		temp = ft_strjoin(result, before); //to_check
// 		free(result);
// 		free(before);
// 		result = temp;
// 		dollar++;
// 		end = dollar;
// 		while (*end && (ft_isalnum(*end) || *end == '_'))
// 			end++;
// 		var_name = ft_strndup(dollar, end - dollar); //to check
// 		var_value = get_env_value(shell, var_name);
// 		temp = ft_strjoin(result, var_value); //to check
// 		free(result);
// 		result = temp;
// 		free(var_name);
// 		start = end;
// 	}
// 	remaining = ft_strdup(start);
// 	temp = ft_strjoin(result, remaining);
// 	free(result);
// 	free(remaining);
// 	result = temp;
// 	return (result);
// }

char *read_heredoc(t_shell *shell, char *delimiter)
{
	char *line;
	char *heredoc_content;
	// char *expanded_line; //N
	(void)shell;

	heredoc_content = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		// if (!shell->heredoc_quoted) //N -> to remove ? 
		// {
		// 	expanded_line = expand_variables(shell, line);
		// 	heredoc_content = ft_strjoin_f(heredoc_content, expanded_line, true);
		// 	free(expanded_line);
		// }
		else
			heredoc_content = ft_strjoin_f(heredoc_content, line, true); //to_check
		heredoc_content = ft_strjoin_f(heredoc_content, "\n", true); // N -> etait a false = leaks
		free(line);
	}
	return (heredoc_content);
}

// faut rajouter le contenu du herdoc dans l'historique -> //N -> non, pas forcement
void handle_heredoc(t_shell *shell, char *delimiter)
{
	int fd;
	char *content;
	ssize_t written;
	char *counter_str;

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

size_t	ft_get_delimiter(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ' 
	&& str[i] != '<' && str[i] != '>'
	&& str[i] != '|')
			i++;
	return (i);
}

char *extract_delimiter(t_shell *shell, char *heredoc_pos)
{
	char *delimiter;
	size_t length;
	char quote_char;
	char *end;

	length = 0;
	if (!*heredoc_pos)
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
	while (*heredoc_pos == ' ')
		heredoc_pos++;
	quote_char = '\0';
	if (*heredoc_pos == '\'' || *heredoc_pos == '"')
	{
		quote_char = *heredoc_pos;
		heredoc_pos++;
		shell->heredoc_quoted = true;
	}
	else
		shell->heredoc_quoted = false;
	
	if (quote_char)
	{	
		length = ft_strlen(heredoc_pos);
		end = ft_strchr(heredoc_pos, quote_char);
		if (end)
			length = end - heredoc_pos;
	}
	else
	{
		length = ft_get_delimiter(heredoc_pos);
		while (length > 0 && heredoc_pos[length - 1] == ' ')
			length--;
	}
	// printf("lennnn : %zu\n", length);
	if (length == 0)
	{
		printf("bash: syntax error near unexpected token `<<'\n"); //"IF NOT IN QUOTE"
		return (NULL);
	}
		
	delimiter = malloc(length + 1);
	if (!delimiter)
		return (NULL);
	ft_strlcpy(delimiter, heredoc_pos, length + 1); // to check ?????
	// printf("final s : %s\n", delimiter);
	return (delimiter);
}
