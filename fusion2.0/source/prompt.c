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

char *path_string(t_shell *shell)
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
    path_end = ft_strjoin_f(path, "\001\033[1;34m\002)> \001\033[0m\002", false);
    free(path);
    return (path_end);
}

char *handle_incomplete_pipe(char *input)
{
	char *temp_input;
	
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

bool	are_quotes_closed(const char *input)
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

char *handle_quotes(t_shell *shell, const char *input)
{
    char *result = malloc(ft_strlen(input) + 1);
    char *res_ptr = result;
    bool single_quote_open = false;
    bool double_quote_open = false;

    while (*input)
    {
        if (*input == '\'' && !double_quote_open)
        {
            single_quote_open = !single_quote_open;
            input++;
            continue;
        }
        else if (*input == '"' && !single_quote_open)
        {
            double_quote_open = !double_quote_open;
            input++;
            continue;
        }
        if (single_quote_open)
            *res_ptr++ = *input++;
        else if (double_quote_open)
        {
            if (*input == '$')
            {
                const char *var_start = input + 1;
                while (ft_isalnum(*var_start) || *var_start == '_')
                    var_start++;
                char *var_name = ft_strndup(input + 1, var_start - (input + 1));
                char *var_value = get_env_value(shell, var_name);
                free(var_name);
                if (var_value)
                {
                    ft_strcpy(res_ptr, var_value);
                    res_ptr += ft_strlen(var_value);
                }
                input = var_start;
            }
            else
                *res_ptr++ = *input++;
        }
        else
            *res_ptr++ = *input++;
    }
    *res_ptr = '\0';
    return result;
}

char *ft_read_input(t_shell *shell)
{
    char *path;
    char *str;
    char *input;
    char *delimiter;
    char *heredoc_pos;

    delimiter = NULL;
    path = path_string(shell);
    str = readline(path);
    free(path);
    if (str == NULL)
        return (NULL);
    if (str[0] != '\0')
        add_history(str);
    input = str;

    //verifie si les cotes sont fermées mais exit minishell a cause du return NULL a modifier
    if (!are_quotes_closed(input))
    {
        printf("Error: Unclosed quotes\n");
        free(input);
        return NULL;
    }

    //gere heredoc avec expansion de variable si cote sur le delimiteur
    heredoc_pos = ft_strnstr(input, "<<", ft_strlen(input));
    if (heredoc_pos)
    {
        heredoc_pos += 2;
        delimiter = extract_delimiter(shell, heredoc_pos);
        handle_heredoc(shell, delimiter);
        free(delimiter);
        input = handle_quotes(shell, input);
    }
    
    //gere incomplete pipe
    while (input && input[ft_strlen(input) - 1] == '|')
        input = handle_incomplete_pipe(input);
    
    return (input);
}




