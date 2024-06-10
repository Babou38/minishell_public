/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 10:13:46 by babou             #+#    #+#             */
/*   Updated: 2024/05/26 15:29:28 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_redirection_out(t_ast_node *node, t_shell *shell)
{
    int fd;
 
    if (node->right && node->right->args)
    {
        fd = open(node->right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            error(NULL, 1);
        int stdout_copy = dup(STDOUT_FILENO);
        if (stdout_copy == -1)
            error(NULL, 1);
        if (dup2(fd, STDOUT_FILENO) == -1)
            error(NULL, 1);
        execute_ast(node->left, shell);
        if (dup2(stdout_copy, STDOUT_FILENO) == -1)
            error(NULL, 1);
        close(fd);
    }
}

void execute_redirection_in(t_ast_node *node, t_shell *shell)
{
    int fd;
    int stdin_copy;

    if (node->right && node->right->args)
    {
        fd = open(node->right->args[0], O_RDONLY);
        if (fd == -1)
            error(NULL, 1);
        stdin_copy = dup(STDIN_FILENO);
        if (stdin_copy == -1)
            error(NULL, 1);
        if (dup2(fd, STDIN_FILENO) == -1)
            error(NULL, 1);
        execute_ast(node->left, shell);
        if (dup2(stdin_copy, STDIN_FILENO) == -1)
            error(NULL, 1);
        close(fd);
    }
}

void execute_extraction(t_ast_node *node, t_shell *shell)
{
    int fd;
 
    if (node->right && node->right->args)
    {
        fd = open(node->right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
            error(NULL, 1);
        int stdout_copy = dup(STDOUT_FILENO);
        if (stdout_copy == -1)
            error(NULL, 1);
        if (dup2(fd, STDOUT_FILENO) == -1)
            error(NULL, 1);
        execute_ast(node->left, shell);
        if (dup2(stdout_copy, STDOUT_FILENO) == -1)
            error(NULL, 1);
        close(fd);
    }
}

void execute_heredoc(t_ast_node *node, t_shell *shell)
{
    int fd;
    int stdin_copy;

    fd = open(shell->heredoc_name, O_RDONLY);
    if (fd == -1)
        error(NULL, 1);
    stdin_copy = dup(STDIN_FILENO);
    if (stdin_copy == -1)
        error(NULL, 1);
    if (dup2(fd, STDIN_FILENO) == -1)
        error(NULL, 1);
    execute_ast(node->left, shell);
    if (dup2(stdin_copy, STDIN_FILENO) == -1)
        error(NULL, 1);
    close(fd);
    unlink(shell->heredoc_name);
    free(shell->heredoc_name);
}

