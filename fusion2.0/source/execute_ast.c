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

#include "../includes/minishell.h"

void printAST(t_ast_node* node)
{
    if (node == NULL)
        return;
    printf("Type: %d ", node->type);
    printf("Command: %s \n", node->value);
    if (node->type == TOKEN_CMD)
    {
        if (node->args != NULL)
        {
            int i = 0;
            while (node->args[i] != NULL)
            {
                printf("Args: %s ", node->args[i]);
                i++;
            }
        }

    }
    printf ("\n");
    printAST(node->left);
    printAST(node->right);
}

void execute_ast(t_ast_node *node, t_shell *shell)
{
    if (node == NULL)
        return ;
    if (node->type == TOKEN_CMD)
        execute_command(node, shell);
    else if (node->type == TOKEN_PIPE)
            execute_pipe(node, shell);
    else if (node->type == TOKEN_REDIRECTION_OUT)
        execute_redirection_out(node, shell);
    else if (node->type == TOKEN_REDIRECTION_IN)
        execute_redirection_in(node, shell);
    else if (node->type == TOKEN_EXTRACTION)
        execute_extraction(node, shell);
    else if (node->type == TOKEN_INSERTION)
        execute_heredoc(node, shell);
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

