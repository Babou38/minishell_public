/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:51:23 by bchapuis          #+#    #+#             */
/*   Updated: 2024/06/17 19:45:55 by babou            ###   ########.fr       */
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

char *handle_quotes(t_shell *shell, char *input)
{
    char	*result = malloc(ft_strlen(input) + 1);
    char	*res_ptr = result;
    bool	single_quote_open = false;
    bool	double_quote_open = false;
	char	*to_free; //N

	to_free = input;
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
                char *var_start = input + 1;
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
	free(to_free);
    return result; //N besoin d'une explication pitie
}

char	*search_heredoc(t_shell *shell, char *input)
{
	char	*delimiter;
	char	*heredoc_pos;
	size_t	i;

	delimiter = NULL;
	i = 0;
	//gere heredoc avec expansion de variable si cote sur le delimiteur
	while (input[i])
	{
		heredoc_pos = ft_strnstr_i(input, "<<", ft_strlen(input), &i);
		if (heredoc_pos)
		{
			heredoc_pos += 2;
	  		delimiter = extract_delimiter(shell, heredoc_pos);
			handle_heredoc(shell, delimiter);
			free(delimiter);
			input = handle_quotes(shell, input);
			i++;
		}
		else
			break;
	}
	//gere incomplete pipe
	while (input && input[ft_strlen(input) - 1] == '|')
		input = handle_incomplete_pipe(input);
	return (input);
}

int	extract_dollar_size(t_shell *shell ,char *str, int begin, int end) //N
{
	char	*to_search;
	char	*var_value;
	int	len_var;
	int k;
	int	j;

	k = begin + 1;
	j = 0;
	len_var = 0;
	to_search = malloc( sizeof(char) * (end - begin + 2));
	if (!to_search)
	{
		//todo secure;
	}
	// printf("i'm at %c..\n", str[k]);
	while (begin++ <= end)
		to_search[j++] = str[k++];
	to_search[j - 1] = '\0';
	// printf("final : %s\n", to_search);
	var_value = ft_getenv(shell->list, to_search);
    if (var_value != NULL)
	{
    	len_var = ft_strlen(var_value);
		// printf("len_var : %d\n", len_var);
		free(var_value);
	}
	free(to_search);
	return (len_var);
}

char	*extract_dollar_value(t_shell *shell ,char *str, int begin, int end) //N
{
	char	*to_search;
	char	*var_value;
	int k;
	int	j;

	k = begin + 1;
	j = 0;
	to_search = malloc( sizeof(char) * (end - begin + 2));
	if (!to_search)
	{
		//todo secure;
	}
	while (begin++ <= end)
		to_search[j++] = str[k++];
	to_search[j - 1] = '\0';
	var_value = ft_getenv(shell->list, to_search);
	free(to_search);
	return (var_value);
}

int		find_malloc_size(t_shell *shell, char *str, int begin, int end)
{
	// int		i;
	// char	*res;
	int		var_size;
	int		to_alloc;

	var_size = 0;
	to_alloc = 0;

	// i = 1;
	// res = str;
	while(str[begin])
	{
		if (contains_global_var(str, &begin, &end, begin))
		{
			var_size += end - begin + 1;
			printf("var size : %d, with start c %c\n",var_size, str[begin]);
			to_alloc += extract_dollar_size(shell, str, begin, end);
			printf("to_ALLOC : %d\n", to_alloc);
			begin = end;
		}
		else
			break;
	}
	return (to_alloc - var_size);
}

char *extracting_doll(t_shell *shell, char *str, char *res, int *i, int *j) //N struct ? 
{
	int	begin;
	int	end;
	int	k;
	char	*value;
	
	end = 0;
	begin = 0;
	k = 0;
	contains_global_var(str, &begin, &end, *i);
	value = extract_dollar_value(shell, str, begin, end);
	while(value && value[k])
	{
		res[(*j)++] = value[k];
		k++;
	}
	free(value);
	*i = end + 1;
	return (res);
}

char	*replace_dollar_value(t_shell *shell, char *str, char *res)
{
	int	i;
	int	j;
	bool can_expand = true;
	
	i = 0;
	j = 0;	
	while(str[i])
	{
		if ((str[i] == '\'') && can_expand) /// a revoir , virer les quotes
			can_expand = false;
		else if ((str[i] == '\'') && !can_expand)
			can_expand = true;
		if (str[i] == '$' && can_expand)
			res = extracting_doll(shell, str, res, &i, &j);
		else
		{
			res[j] = str[i];
			j++;
			i++;
			res[j] = '\0';
		}
	}
	res[j] = '\0';
	printf("res : (%s)\n", res);
	return (res);
}

char	*expand_input(t_shell *shell, char *str)
{
	char	*res = NULL;
	int	base_size;
	int	to_alloc;
	(void)shell;

	
	base_size = ft_strlen(str);
	to_alloc = find_malloc_size(shell, str, 0, 0);
	// printf("final size : %d + %d + 1 = %d\n", base_size , to_alloc, base_size + to_alloc + 1);
	res = malloc(sizeof(char) * (base_size + to_alloc + 1));
	res = replace_dollar_value(shell, str, res);
	free(str);
	return (res);
}

char *ft_read_input(t_shell *shell)
{
    char *path;
    char *str;
    char *input;

    path = path_string(shell);
    str = readline(path);
	free(path);
	if (str != NULL && str[0]) //N
		add_history(str);
	if (str == NULL)
		return (NULL);
	input = ft_strdup(str); //N
	free(str);
	//verifie si les cotes sont fermées mais exit minishell a cause du return NULL a modifier
	if (!are_quotes_closed(input))
	{
		printf("Error: Unclosed quotes\n");
		free(input);
		return NULL;
	}
	input = expand_input(shell, input); 
	input = search_heredoc(shell, input);
	return (input);
}