/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:20:59 by marvin            #+#    #+#             */
/*   Updated: 2024/04/22 17:20:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_command(t_shell *shell)
{
    if (shell->ast->value != NULL && strlen(shell->ast->value) > 0)
    {
        if (ft_strncmp(shell->ast->value, "exit", 4) == 0)
            shell->exit = ft_is_exit(shell);
        else if (ft_strncmp(shell->ast->value, "cd", 2) == 0)
            ft_cd(shell);
        else if (ft_strncmp(shell->ast->value, "pwd", 3) == 0)
            ft_pwd(shell);
        else if (ft_strncmp(shell->ast->value, "echo", 4) == 0)
            ft_echo(shell);
        else if (ft_strncmp(shell->ast->value, "env", 3) == 0)
            ft_env(shell);
        else if (ft_strncmp(shell->ast->value, "unset", 5) == 0)
            ft_unset(shell);
        else if (ft_strncmp(shell->ast->value, "export", 6) == 0)
            ft_export(shell);
        else
            ft_execute(shell->ast->args, shell->env, shell);
    }
}


void execute_command(t_ast_node *node, t_shell *shell)
{
    if (node->type == TOKEN_CMD)
    {
        shell->ast = node;
        handle_command(shell);
    }
}