/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:22:59 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:22:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_shell *shell)
{
    char *path;
    if (shell->cmd[0] != NULL && ft_strncmp(shell->cmd[0], "cd", 2) == 0)
    {
        if (shell->cmd[1] == NULL)
        {
            path = getenv("HOME");
            if (chdir(path) != 0)
                perror("cd failed");
        }
        else
        {
            if (chdir(shell->cmd[1]) != 0)
                perror("cd failed");
        }
    }
    
}