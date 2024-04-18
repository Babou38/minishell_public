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
    char *env_value;

    if (shell->cmd[i] != NULL && ft_strncmp(shell->cmd[i], "-n", 2) == 0)
    {
        newline = 0;
        i++;
    }
    while (shell->cmd[i] != NULL)
    {
        if (ft_strncmp(shell->cmd[i], "$?", 2) == 0)
            printf("%d ", shell->exitcode);
        else if (shell->cmd[i][0] == '$')
        {
           env_value = getenv(shell->cmd[i] + 1);
            if (env_value != NULL)
                printf("%s ", env_value);
        }
        else
            printf("%s ", shell->cmd[i]);
        i++;
    }
    if (newline)
        printf("\n");
    shell->exitcode = 0;
}