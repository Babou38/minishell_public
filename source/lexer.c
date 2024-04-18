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

    tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));
    if (tokens == NULL)
        exit(1);
    i = 0;
    j = 0;
    while (input[i])
    {
        if (input[i] != ' ')
        {
            if (input[i] == '|')
            {
                tokens[j++] = create_token(TOKEN_PIPE, "|");
                i++;
            }
            else
            {
                k = i;
                while (input[k] && input[k] != ' ' && input[k] != '|')
                    k++;
                word = strndup(input + i, k - i);
                if (!word)
                    exit(1);
                tokens[j++] = create_token(TOKEN_WORD, word);
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
        printf("Token %d:\n", i);
        printf("Type: %d\n", tokens[i]->type);
        printf("Value: %s\n\n", tokens[i]->value);
        i++;
    }
}

void free_tokens(t_token **tokens)
{
    int i = 0;

    while (tokens[i])
    {
        free(tokens[i]->value);
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

