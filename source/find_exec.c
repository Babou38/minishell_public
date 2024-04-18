/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:30:18 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:30:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_execute(char **cmd, char **envp, t_shell *shell)
{
    pid_t   pid;
    char *path;
    int     status;

    if (cmd[0] == NULL)
        return;
    pid = fork();
    if (pid == -1)
        perror("fork failed");
    else if (pid == 0)
    {
        path = get_cmd_path(cmd[0], envp);
        if (execve(path, cmd, envp) == -1)
        {
            printf("%s: command not found\n", cmd[0]);
            free(path);
            exit(127);
        }
        free(path);
        exit(0);
    }
    waitpid(pid, &status, 0);
    shell->exitcode = status >> 8;
}

char    *join_path_cmd(char *path, char *cmd)
{
        char    *tmp;
        char    *full_path;

        tmp = ft_strjoin(path, "/");
        if (tmp == NULL)
        {
                free(path);
                error(NULL);
        }
        full_path = ft_strjoin(tmp, cmd);
        if (full_path == NULL)
        {
                free(tmp);
                free(path);
                error(NULL);
        }
        free(tmp);
        return (full_path);
}

char    *check_path(char **path, char *cmd)
{
        int             i;
        char    *full_path;

        i = -1;
        while (path[++i])
        {
                full_path = join_path_cmd(path[i], cmd);
                if (access(full_path, X_OK) == 0)
                        return (ft_free_split(path), full_path);
                free(full_path);
        }
        ft_free_split(path);
        return (NULL);
}

char    *get_cmd_path(char *cmd, char **env)
{
        int             i;
        char    **path;

        i = -1;
        if (access(cmd, X_OK) == 0)
                return (cmd);
        else if (cmd[0] == '.' && cmd[1] == '/')
        {
                cmd++;
                cmd++;
                if (access(cmd, X_OK) == 0)
                        return (cmd);
                else
                        error(NULL);
        }
        while (env[++i])
                if (!ft_strncmp(env[i], "PATH=", 5))
                        break ;
        if (env[i] == NULL)
                ft_no_path();
        path = ft_split(env[i] + 5, ':');
        if (path == NULL)
                error(NULL);
        return (check_path(path, cmd));
}
