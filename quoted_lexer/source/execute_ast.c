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

void execute_ast(t_ast_node *node, t_shell *shell)
{
	// if (node)
	// 	printf("je suis %s\n", node->value);
    if (node == NULL)
        return ;
    if (node->type == TOKEN_CMD)
        handle_command(node, shell);
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
		return ;
	if (node->left)
		free_ast(node->left);
	// else
	// 	printf("\033[1;34mno left node found for %s...\033[0m\n", node->value);
	if (node->right)
		free_ast(node->right);
	// else
	// 	printf("\033[1;34mno right node found for %s...\033[0m\n", node->value);
	// printf("freeing node ! \n");
	if (node->args != NULL)
	{
		// printf("arg is :  %s ! \n", node->args[0]);
		ft_free_split(node->args);
	}
	if (node->value)
	{
		// printf("value is %s ! \n", node->value);
		free(node->value);
	}
	free(node);
}

