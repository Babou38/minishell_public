/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 13:50:21 by marvin            #+#    #+#             */
/*   Updated: 2024/04/14 13:50:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_unset(t_shell *shell)
{
    char **env_ptr;
    char **next_env_ptr;

    env_ptr = shell->env;
    while (*env_ptr && ft_strncmp(*env_ptr, shell->cmd[1], ft_strlen(shell->cmd[1])) != 0)
        env_ptr++;
    if (*env_ptr)
    {
        free(*env_ptr);
        next_env_ptr = env_ptr;
        while (*next_env_ptr)
        {
            *next_env_ptr = *(next_env_ptr + 1);
            next_env_ptr++;
        }
    }
    shell->exitcode = 0;
}