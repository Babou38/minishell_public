/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 14:19:02 by marvin            #+#    #+#             */
/*   Updated: 2024/04/14 14:19:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_export(t_shell *shell)
{
    int i;
    int j;
    int k;
    int exist;
    char **new_env;

    i = 0;
    j = 0;
    k = 0;
    exist = 0;
    char *ptr = ft_strchr(shell->ast->args[1], '=');
    if (ptr != NULL)
    {
        while (shell->ast->args[1][j] != *ptr)
            j++;
        j++;
        while (shell->env[++i])
        {
            if (ft_strncmp(shell->env[i], shell->ast->args[1], j) == 0)
            {
                exist = 1;
                free(shell->env[i]);
                shell->env[i] = ft_strdup(shell->ast->args[1]);
                break ;
            }
            else
                exist = 0;
        }
         if (exist == 0)
        {
            new_env = malloc(sizeof(char *) * (i + 2));
            while (k < i)
            {
                new_env[k] = shell->env[k];
                k++;
            }
            new_env[i] = ft_strdup(shell->ast->args[1]);
            new_env[i + 1] = NULL;
            free(shell->env);
            shell->env = new_env;
        }
        shell->exitcode = 0;
    }
}