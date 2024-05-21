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

void update_env_var(char **env, char *var, char *new_value)
{
        int i;
    int j;

    i = 0;
    j = 0;
    char *ptr = ft_strchr(var, '=');
    if (ptr != NULL)
    {
        while (var[j] != *ptr)
            j++;
        j++;
        while (env[++i])
        {
            if (ft_strncmp(env[i], var, j) == 0)
            {
                free(env[i]);
                env[i] = ft_strjoin(var, new_value);
                break ;
            }
        }
    }
}

void ft_cd(t_shell *shell)
{
    char *path;
    char *oldpwd;
    char *newpwd;

    oldpwd = getcwd(NULL, 0);
    if (shell->ast->args[1] == NULL)
    {
        path = getenv("HOME");
        if (chdir(path) != 0)
            perror("cd failed");
    }
    else
    {
        if (chdir(shell->ast->args[1]) != 0)
               perror("cd failed");
    }
    newpwd = getcwd(NULL, 0);
    update_env_var(shell->env, "OLDPWD=", oldpwd);
    update_env_var(shell->env, "PWD=", newpwd);
    free(oldpwd);
    free(newpwd);
    shell->exitcode = 0;
}
