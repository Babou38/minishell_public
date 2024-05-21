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

#include "minishell.h"

t_token *create_token(t_tokentype type, char *value)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (token == NULL)
        exit(1);
    token->type = type;
    token->value = ft_strdup(value);
    if (token->value == NULL)
        exit(1);
    return (token);
}

t_token **lexer(char *input)
{
    t_token **tokens;
    char *word;
    int i, j, k;
    int is_command;

    tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));
    if (tokens == NULL)
        exit(1);
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
            else if (input[i] == 39)
            {
                tokens[j++] = create_token(TOKEN_SINGLE_QUOTE, "\'");
                i++;
            }
            else if (input[i] == 34)
            {
                tokens[j++] = create_token(TOKEN_DOUBLE_QUOTE, "\"");
                i++;
            }
            else
            {
                k = i;
                while (input[k] && input[k] != ' ' && input[k] != '|' && input[k] != '<' && input[k] != '>')
                    k++;
                word = ft_strndup(input + i, k - i);
                if (!word)
                    exit(1);
                if (is_command)
                {
                    tokens[j++] = create_token(TOKEN_CMD, word);
                    is_command = 0;
                }
                else
                {
                    tokens[j++] = create_token(TOKEN_ARG, word);
                }
                free(word);
                i = k;
            }
        }
        else
            i++;
    }
    tokens[j] = NULL;
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
