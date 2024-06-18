/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:19:42 by babou             #+#    #+#             */
/*   Updated: 2024/06/17 18:21:07 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast_node *create_ast_node(t_token *token, bool debug)
{
	t_ast_node *node;

	if (debug)
	{
		printf("------------create ast node-----------\n");
		printf("\ntoken->value = %s\n", token->value);
		printf("token->type = %u\n\n", token->type);
	}
    if (token == NULL)
        return NULL;
    node = malloc(sizeof(t_ast_node)); // malloc a check
    if (node == NULL)
        exit(1);
    node->left = NULL;
    node->right = NULL;
    node->args = NULL;
	// if (token->type == TOKEN_CMD)
	// {
	// 	node->args = malloc(sizeof(token->value));
	// 	node->args[0] = ft_strdup(token->value);
	// }
    node->type = token->type;
    node->value = ft_strdup(token->value);
    if (node->value == NULL) // malloc a check
        exit(1);
    return node;
}

void add_argument_to_command(t_ast_node *command_node, char *argument, bool debug)
{
    int i;
    int k;
    char **new_args;

    i = 0;
    k = 0;
	if (debug)
	{
		printf("------------add arg cmd---------------\n");
		printf("\nargument : %s\n\n", argument);
	}
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

t_ast_node *create_command_node(t_token **tokens, int *i, t_ast_node *first, bool debug)
{
    t_ast_node *command_node;
    (void)first;
	if (debug)
	{
		printf("------------create cmd node-----------\n");
		printf("\ntoken[%d]-> value = %s\n\n", *i, tokens[*i]->value);
	}
	// if (*i == 0)
	// 	command_node = first;
	// else
    	command_node = create_ast_node(tokens[*i], debug);
    while (tokens[*i] != NULL && tokens[*i]->type != TOKEN_PIPE && tokens[*i]->type != TOKEN_REDIRECTION_OUT && tokens[*i]->type != TOKEN_REDIRECTION_IN && tokens[*i]->type != TOKEN_EXTRACTION && tokens[*i]->type != TOKEN_INSERTION)
    {
        if (command_node->args == NULL)
        {
            command_node->args = malloc(sizeof(char *) * 2);
            command_node->args[0] = ft_strdup(tokens[*i]->value);
            command_node->args[1] = NULL;
        }
        // if (tokens[*i]->type == TOKEN_ARG)
        //     add_argument_to_command(command_node, tokens[*i]->value, debug);
        if (tokens[*i]->type == TOKEN_ARG || tokens[*i]->type == TOKEN_SINGLE_QUOTE || tokens[*i]->type == TOKEN_DOUBLE_QUOTE)
            add_argument_to_command(command_node, tokens[*i]->value, debug);
        (*i)++;
    }
    return command_node;
}

t_ast_node *handle_token_type(t_token **tokens, int *i, t_ast_node *node, bool debug)
{
    t_ast_node *new_node;
    t_ast_node *command_node;
    
	if (debug)
	{
		printf("-----special symbol ! -> handle_token_type----------\n");
		printf("\ntoken type : %u\n\n", tokens[*i]->type);
	}
    new_node = create_ast_node(tokens[*i], debug);
	if (*i != 0)
    	new_node->left = node;
	else
	{
		free(node->value);
		free(node);
	}
    (*i)++;
    if (tokens[*i] != NULL && tokens[*i]->type != tokens[*i - 1]->type) //si pas deux commandes a la suite
    {
        command_node = create_command_node(tokens, i, node, debug);
		if (debug)
		{
			printf("****adding cmd node !****\n");
			printf("\n%s now has %s on his right\n\n", new_node->value, command_node->value);
		}
        new_node->right = command_node;
    }
    return new_node;
}

t_ast_node *build_ast(t_token **tokens, bool debug)
{
    t_ast_node *node;
    t_ast_node *command_node;
	int i;

    if (debug)
		printf("BUILDING AST :\n\n");
    node = create_ast_node(tokens[0], debug); //N POURQUOI
    i = 0; //N
    if (tokens == NULL || tokens[0] == NULL)
       return NULL;
    while (tokens[i] != NULL)
    {
        if (tokens[i]->type == TOKEN_PIPE || tokens[i]->type == TOKEN_REDIRECTION_OUT || tokens[i]->type == TOKEN_REDIRECTION_IN || tokens[i]->type == TOKEN_EXTRACTION || tokens[i]->type == TOKEN_INSERTION)
            node = handle_token_type(tokens, &i, node, debug);
        else if (tokens[i]->type == TOKEN_CMD)
        {
			free(node->value); 
			free(node);
            command_node = create_command_node(tokens, &i, node, debug);
            command_node->left = NULL;
            command_node->right = NULL;
            node = command_node;
        }
		// else //N
		// {
		// 	add_argument_to_command(node, tokens[i - 1]->value, debug);
		// 	add_argument_to_command(node, tokens[i]->value, debug);
		// 	i++;
		// }
    }
    return node;
}
