/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:06:24 by marvin            #+#    #+#             */
/*   Updated: 2024/04/19 17:06:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_ast_node *build_ast(t_token **tokens)
{
    t_ast_node *node = create_ast_node(tokens[0]);
    int i = 0;

    if (tokens == NULL || tokens[0] == NULL)
       return NULL;
    while (tokens[i] != NULL)
    {
        if (tokens[i]->type == TOKEN_PIPE)
        {
            t_ast_node *pipe_node = create_ast_node(tokens[i]);
            pipe_node->left = node;
            i++;
            if (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE)
            {
                t_ast_node *command_node = create_ast_node(tokens[i]);
                while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE)
                {
                    if (command_node->args == NULL)
                    {
                        command_node->args = malloc(sizeof(char *) * 2);
                        command_node->args[0] = ft_strdup(tokens[i]->value);
                        command_node->args[1] = NULL;
                    }
                    if (tokens[i]->type == TOKEN_ARG)
                        add_argument_to_command(command_node, tokens[i]->value);
                    i++;
                }
                pipe_node->right = command_node;
                node = pipe_node;
            }
        }
        else if (tokens[i]->type == TOKEN_REDIRECTION_IN || tokens[i]->type == TOKEN_REDIRECTION_OUT)
        {
            t_ast_node *redirection_node = create_ast_node(tokens[i]);
            redirection_node->left = node;
            i++;
            t_ast_node *command_node = create_ast_node(tokens[i]);
            while (tokens[i] != NULL && tokens[i]->type != TOKEN_PIPE && tokens[i]->type != TOKEN_REDIRECTION_IN && tokens[i]->type != TOKEN_REDIRECTION_OUT)
            {
                if (tokens[i]->type == TOKEN_ARG)
                    add_argument_to_command(command_node, tokens[i]->value);
                i++;
            }
            redirection_node->right = command_node;
            node = redirection_node;
        }
        else if (tokens[i]->type == TOKEN_CMD)
        {
            t_ast_node *command_node = create_ast_node(tokens[i]);
            command_node->left = NULL;
            command_node->right = NULL;
            if (command_node->args == NULL)
            {
                command_node->args = malloc(sizeof(char *) * 2);
                command_node->args[0] = ft_strdup(tokens[i]->value);
                command_node->args[1] = NULL;
            }
            while (tokens[i + 1] && tokens[i + 1]->type == TOKEN_ARG)
            {
                add_argument_to_command(command_node, tokens[i + 1]->value);
                i++;
            }
            node = command_node;
            i++;
        }
    }
    return node;
}

void execute_ast(t_ast_node *node, t_shell *shell)
{
    if (node == NULL)
        return ;
    if (node->type == TOKEN_CMD)
    {
        execute_command(node, shell);
    }
    else if (node->type == TOKEN_PIPE)
    {
        execute_pipe(node, shell);
        // execute_ast(node->right, shell);
    }
    // else if (node->type == TOKEN_REDIRECTION_IN || node->type == TOKEN_REDIRECTION_OUT)
    // {
    //     // execute_redirection(node, shell);
    // }
    else
    {
        execute_ast(node->left, shell);
        execute_ast(node->right, shell);
    }
}

void free_ast(t_ast_node *node)
{
    if (node == NULL)
        return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->args != NULL)
        ft_free_split(node->args);
    free(node->value);
    free(node);
}

