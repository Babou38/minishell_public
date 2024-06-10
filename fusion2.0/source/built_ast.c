/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:19:42 by babou             #+#    #+#             */
/*   Updated: 2024/05/26 12:27:49 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast_node *create_ast_node(t_token *token)
{
    if (token == NULL)
        return NULL;
    
    t_ast_node *node = malloc(sizeof(t_ast_node));
    if (node == NULL)
        exit(1);
    node->left = NULL;
    node->right = NULL;
    node->args = NULL;
    node->type = token->type;
    node->value = ft_strdup(token->value);
    if (node->value == NULL)
        exit(1);
    return node;
}

void add_argument_to_command(t_ast_node *command_node, char *argument)
{
    int i;
    int k;
    char **new_args;

    i = 0;
    k = 0;
    if (command_node->args == NULL)
    {
        command_node->args = malloc(sizeof(char *) * 2);
        command_node->args[0] = ft_strdup(argument);
        command_node->args[1] = NULL;
    }
    else
    {
        while (command_node->args[i])
            i++;
        new_args = malloc(sizeof(char *) * (i + 2));
        while (k < i)
        {
            new_args[k] = command_node->args[k];
            k++;
        }
            new_args[i] = ft_strdup(argument);
            new_args[i + 1] = NULL;
            free(command_node->args);
            command_node->args = new_args;
    }
}

t_ast_node *create_command_node(t_token **tokens, int *i)
{
    t_ast_node *command_node;
    
    command_node = create_ast_node(tokens[*i]);
    while (tokens[*i] != NULL && tokens[*i]->type != TOKEN_PIPE && tokens[*i]->type != TOKEN_REDIRECTION_OUT && tokens[*i]->type != TOKEN_REDIRECTION_IN && tokens[*i]->type != TOKEN_EXTRACTION && tokens[*i]->type != TOKEN_INSERTION)
    {
        if (command_node->args == NULL)
        {
            command_node->args = malloc(sizeof(char *) * 2);
            command_node->args[0] = ft_strdup(tokens[*i]->value);
            command_node->args[1] = NULL;
        }
        if (tokens[*i]->type == TOKEN_ARG)
            add_argument_to_command(command_node, tokens[*i]->value);
        (*i)++;
    }
    return command_node;
}

t_ast_node *handle_token_type(t_token **tokens, int *i, t_ast_node *node)
{
    t_ast_node *new_node;
    t_ast_node *command_node;
    
    new_node = create_ast_node(tokens[*i]);
    new_node->left = node;
    (*i)++;
    if (tokens[*i] != NULL && tokens[*i]->type != tokens[*i - 1]->type)
    {
        command_node = create_command_node(tokens, i);
        new_node->right = command_node;
    }
    return new_node;
}

t_ast_node *build_ast(t_token **tokens)
{
    t_ast_node *node;
    t_ast_node *command_node;
    
    node = create_ast_node(tokens[0]);
    int i = 0;

    if (tokens == NULL || tokens[0] == NULL)
       return NULL;
    while (tokens[i] != NULL)
    {
        if (tokens[i]->type == TOKEN_PIPE || tokens[i]->type == TOKEN_REDIRECTION_OUT || tokens[i]->type == TOKEN_REDIRECTION_IN || tokens[i]->type == TOKEN_EXTRACTION || tokens[i]->type == TOKEN_INSERTION)
            node = handle_token_type(tokens, &i, node);
        else if (tokens[i]->type == TOKEN_CMD)
        {
            command_node = create_command_node(tokens, &i);
            command_node->left = NULL;
            command_node->right = NULL;
            node = command_node;
        }
    }
    return node;
}






































































// t_ast_node *build_ast(t_token **tokens)
// {
//     t_ast_node *node = create_ast_node(tokens[0]);
//     int i = 0;

//     if (tokens == NULL || tokens[0] == NULL)
//        return NULL;
//     while (tokens[i] != NULL)
//     {
//         if (tokens[i]->type == TOKEN_PIPE)
//         {
//             t_ast_node *pipe_node = create_ast_node(tokens[i]);
//             pipe_node->left = node;
//             i++;
//             if (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE)
//             {
//                 t_ast_node *command_node = create_ast_node(tokens[i]);
//                 while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE && tokens[i]->type != TOKEN_REDIRECTION_OUT && tokens[i]->type != TOKEN_REDIRECTION_IN && tokens[i]->type != TOKEN_EXTRACTION)
//                 {
//                     if (command_node->args == NULL)
//                     {
//                         command_node->args = malloc(sizeof(char *) * 2);
//                         command_node->args[0] = ft_strdup(tokens[i]->value);
//                         command_node->args[1] = NULL;
//                     }
//                     if (tokens[i]->type == TOKEN_ARG)
//                         add_argument_to_command(command_node, tokens[i]->value);
//                     i++;
//                 }
//                 pipe_node->right = command_node;
//                 node = pipe_node;
//             }
//         }
//         else if (tokens[i]->type == TOKEN_REDIRECTION_OUT)
//         {
//             t_ast_node *redirection_node = create_ast_node(tokens[i]);
//             redirection_node->left = node;
//             i++;
//             if (tokens[i] != NULL && tokens[i]->type != TOKEN_REDIRECTION_OUT)
//             {
//                 t_ast_node *command_node = create_ast_node(tokens[i]);
//                 while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE && tokens[i]->type != TOKEN_REDIRECTION_OUT && tokens[i]->type != TOKEN_REDIRECTION_IN && tokens[i]->type != TOKEN_EXTRACTION)
//                 {
//                     if (command_node->args == NULL)
//                     {
//                         command_node->args = malloc(sizeof(char *) * 2);
//                         command_node->args[0] = ft_strdup(tokens[i]->value);
//                         command_node->args[1] = NULL;
//                     }
//                     if (tokens[i]->type == TOKEN_ARG)
//                         add_argument_to_command(command_node, tokens[i]->value);
//                     i++;
//                 }
//                 redirection_node->right = command_node;
//                 node = redirection_node;
//             }
//         }
//         else if (tokens[i]->type == TOKEN_REDIRECTION_IN)
//         {
//             t_ast_node *redirection_node = create_ast_node(tokens[i]);
//             redirection_node->left = node;
//             i++;
//             if (tokens[i] != NULL && tokens[i]->type != TOKEN_REDIRECTION_IN)
//             {
//                 t_ast_node *command_node = create_ast_node(tokens[i]);
//                 while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE && tokens[i]->type != TOKEN_REDIRECTION_OUT && tokens[i]->type != TOKEN_REDIRECTION_IN && tokens[i]->type != TOKEN_EXTRACTION)
//                 {
//                     if (command_node->args == NULL)
//                     {
//                         command_node->args = malloc(sizeof(char *) * 2);
//                         command_node->args[0] = ft_strdup(tokens[i]->value);
//                         command_node->args[1] = NULL;
//                     }
//                     if (tokens[i]->type == TOKEN_ARG)
//                         add_argument_to_command(command_node, tokens[i]->value);
//                     i++;
//                 }
//                 redirection_node->right = command_node;
//                 node = redirection_node;
//             }
//         }
//         else if (tokens[i]->type == TOKEN_EXTRACTION)
//         {
//             t_ast_node *redirection_node = create_ast_node(tokens[i]);
//             redirection_node->left = node;
//             i++;
//             if (tokens[i] != NULL && tokens[i]->type != TOKEN_EXTRACTION)
//             {
//                 t_ast_node *command_node = create_ast_node(tokens[i]);
//                 while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE && tokens[i]->type != TOKEN_REDIRECTION_OUT && tokens[i]->type != TOKEN_REDIRECTION_IN && tokens[i]->type != TOKEN_EXTRACTION)
//                 {
//                     if (command_node->args == NULL)
//                     {
//                         command_node->args = malloc(sizeof(char *) * 2);
//                         command_node->args[0] = ft_strdup(tokens[i]->value);
//                         command_node->args[1] = NULL;
//                     }
//                     if (tokens[i]->type == TOKEN_ARG)
//                         add_argument_to_command(command_node, tokens[i]->value);
//                     i++;
//                 }
//                 redirection_node->right = command_node;
//                 node = redirection_node;
//             }
//         }
//         else if (tokens[i]->type == TOKEN_CMD)
//         {
//              t_ast_node *command_node = create_ast_node(tokens[i]);
//             command_node->left = NULL;
//             command_node->right = NULL;
//             if (command_node->args == NULL)
//             {
//                 command_node->args = malloc(sizeof(char *) * 2);
//                 command_node->args[0] = ft_strdup(tokens[i]->value);
//                 command_node->args[1] = NULL;
//             }
//             while (tokens[i + 1] && tokens[i + 1]->type == TOKEN_ARG)
//             {
//                 add_argument_to_command(command_node, tokens[i + 1]->value);
//                 i++;
//             }
//             node = command_node;
//             i++;
//         }
//     }
//     return node;
// }























