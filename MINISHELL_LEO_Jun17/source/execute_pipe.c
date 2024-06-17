/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:58:17 by marvin            #+#    #+#             */
/*   Updated: 2024/05/01 19:58:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_pipe(t_ast_node *node, t_shell *shell)
{
    int pipefd[2];
	pid_t left_pid;
	pid_t right_pid;
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    left_pid = fork();
    if (left_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (left_pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execute_ast(node->left, shell);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    } /// faire attendre ?
    right_pid = fork();
    if (right_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (right_pid == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execute_ast(node->right, shell);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}
