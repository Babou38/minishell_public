/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:24:19 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:24:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_shell *shell)
{
    int i = 1;
    int newline = 1;

    if (shell->cmd[0] != NULL && ft_strncmp(shell->cmd[0], "echo", 4) == 0)
    {
        if (shell->cmd[i] != NULL && ft_strncmp(shell->cmd[i], "-n", 2) == 0)
        {
            newline = 0;
            i++;
        }
        while (shell->cmd[i] != NULL)
        {
            printf("%s ", shell->cmd[i]);
            i++;
        }
        if (newline)
            printf("\n");
    }
}