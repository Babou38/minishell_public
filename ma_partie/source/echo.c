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

    if (shell->ast->args[i] != NULL && ft_strncmp(shell->ast->args[i], "-n", 2) == 0)
    {
        newline = 0;
        i++;
    }
    while (shell->ast->args[i] != NULL)
    {
        if (ft_strncmp(shell->ast->args[i], "$?", 2) == 0)
            printf("%d ", shell->exitcode);
        else if (shell->ast->args[i][0] == '$')
        {
           env_value = getenv(shell->ast->args[i] + 1);
            if (env_value != NULL)
                printf("%s ", env_value);
        }
        else
            printf("%s ", shell->ast->args[i]);
        i++;
    }
    if (newline)
        printf("\n");
    shell->exitcode = 0;
}