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

#include "../includes/minishell.h"

void handle_command(t_ast_node *node, t_shell *shell)
{
    if (node->value != NULL && strlen(node->value) > 0)
    {
        if (ft_strcmp(node->value, "exit") == 0)
            shell->exit = ft_is_exit(shell, node);
        else if (ft_strcmp(node->value, "cd") == 0)
            ft_cd(shell, node);
        else if (ft_strcmp(node->value, "pwd") == 0)
            ft_pwd(shell, node);
        else if (ft_strcmp(node->value, "echo") == 0)
            ft_echo(shell, node);
        else if (ft_strcmp(node->value, "env") == 0)
            ft_env(shell, node);
        else if (ft_strcmp(node->value, "unset") == 0)
            ft_unset(shell, node);
        else if (ft_strcmp(node->value, "export") == 0)
            ft_export(shell, node);
        else
            ft_execute(node->args, shell->env, shell);
    }
}

//N  -> on fait direct handle_command

// void execute_command(t_ast_node *node, t_shell *shell)
// {
//     if (node->type == TOKEN_CMD)
//     {
//         // shell->ast = node;    // cassait TOUUUUUUT en changeant le pointeur

// 		// ---> a cause de cette ligne, si on executait "wc -l < Makefile", alors shell.ast va pointer sur "wc", ce 
// 		// qui empeche le free de foncionner correctement puisqu'il va se derouler sur ses enfants 
// 		// (left node et right node, tout deux inexistants), puis se free lui meme, laissant "Makefile" et "<"

//         handle_command(node, shell);
//     }
// }