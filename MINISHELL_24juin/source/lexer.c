/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 09:37:26 by marvin            #+#    #+#             */
/*   Updated: 2024/04/18 09:37:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token *create_token(t_tokentype type, char *value)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (token == NULL)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    if (token->value == NULL)
    {
		free(token);
		return (NULL);
	}
    return (token);
}

t_token **lexer(char *input, bool *is_valid) //N
{
    t_token **tokens;
    char *word;
    int i, j, k;
    int is_command;

    tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));  /// to check
    if (tokens == NULL)
	{	
		*is_valid = false;
		return tokens;
	}
    i = 0;
    j = 0;
    is_command = 1;
    while (input[i])
    {
        if (input[i] != ' ')
        {
            if (input[i] == '|')
            {
                tokens[j++] = create_token(TOKEN_PIPE, "|");
                i++;
                is_command = 1;
            }
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                tokens[j++] = create_token(TOKEN_INSERTION, "<<");
                i = i + 2;
				is_command = 0; //N
            }
            else if (input[i] == '>' && input[i + 1] == '>')
            {
                tokens[j++] = create_token(TOKEN_EXTRACTION, ">>");
                i = i + 2;
            }
            else if (input[i] == '<')
            {
                tokens[j++] = create_token(TOKEN_REDIRECTION_IN, "<");
                i++;
                is_command = 1;
            }
            else if (input[i] == '>')
            {
                tokens[j++] = create_token(TOKEN_REDIRECTION_OUT, ">");
                i++;
                is_command = 1;
            }
            else if (input[i] == '\'' || input[i] == '\"')
            {
                char quote = input[i];
                k = ++i;
                while (input[i] && input[i] != quote)
                    i++;
                word = ft_strndup(input + k, i - k); // to check
                if (quote == '\'')
                    tokens[j++] = create_token(TOKEN_SINGLE_QUOTE, word);
                else
                    tokens[j++] = create_token(TOKEN_DOUBLE_QUOTE, word);
                free(word);
                i++;
            }
            else
            {
                k = i;
                while (input[k] && input[k] != ' ' && input[k] != '|' && input[k] != '<' && input[k] != '>' && input[k] != '\'' && input[k] != '"')
                    k++;
                word = ft_strndup(input + i, k - i);
                if (!word)
				{
					*is_valid = false;
					tokens[j] = NULL;
					return (tokens);
				}
                if (is_command)
                {
                    tokens[j++] = create_token(TOKEN_CMD, word);
                    is_command = 0;
                }
                else
                {
                    if (j >= 2 && (tokens[j - 2]->type == TOKEN_REDIRECTION_IN || tokens[j - 2]->type == TOKEN_INSERTION))
                            tokens[j++] = create_token(TOKEN_CMD, word);
                    else
                        tokens[j++] = create_token(TOKEN_ARG, word);
                }
                free(word);
                i = k;
            }
			if (tokens[j - 2] == NULL)
			{
				printf("i am tokens[%d] and NULL\n", j - 1);
				*is_valid = false;
				return (tokens);
			}
        }
        else
            i++;
    }
    tokens[j] = NULL;
	*is_valid = true;
    return (tokens);
}

void print_tokens(t_token **tokens)
{
    int i = 0;

    while (tokens[i])
    {
        printf("Token %d: ", i);
        printf("Type: %d ", tokens[i]->type);
        printf("Value: %s\n", tokens[i]->value);
        i++;
    }
}
