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
    if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "export", 6) == 0)
    {
        char *ptr = ft_strchr(shellshell.cmd[1])
        if (ptr != NULL)
        {
            free(ptr);
            printf("yes");
        }
    }
}