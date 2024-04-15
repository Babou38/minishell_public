/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:24:59 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:24:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(t_shell *shell)
{
    int i = 0;
    if (shell->cmd[0] != NULL && ft_strncmp(shell->cmd[0], "env", 3) == 0)
    {
        while (shell->env[i] != NULL)
        {
            printf("%s\n", shell->env[i]);
            i++;
        }
    }
}
