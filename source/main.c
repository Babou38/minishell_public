/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:49 by bchapuis          #+#    #+#             */
/*   Updated: 2024/04/15 13:58:38 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_read_input(void)
{
    char *pwd;
    char *str;
    pwd = getcwd(NULL, 0);
    printf("\033[1;34mminishell(\033[0m\033[1;31m%s\033[0m", pwd);
    str = readline("\033[1;34m)> \033[0m");
    free(pwd);
    add_history(str);
    return(str);
}

void init_envp(t_shell *shell, char **envp)
{
    size_t len = 0;
    size_t i = 0;
    while (envp[len])
        len++;
    shell->env = malloc((len + 1) * sizeof(char *));
    while (i < len)
    {
        shell->env[i] = ft_strdup(envp[i]);
        i++;
    }
    shell->env[len] = NULL;
}

char **ft_parsing(t_shell *shell)
{
    char **cmd;
    cmd = ft_split(shell->str, ' ');
    if (cmd == NULL)
        exit(0);
    return (cmd);
}

int main (int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    shell.exit = 0;
    init_envp(&shell, envp);
    while(shell.exit == 0)
    {
        shell.str = ft_read_input();
        if (shell.str != NULL)
        {
            shell.cmd = ft_parsing(&shell);
            if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "exit", 4) == 0)
                shell.exit = ft_is_exit(&shell);
            else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "cd", 2) == 0)
                ft_cd(&shell);
            else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "pwd", 3) == 0)
                ft_pwd(&shell);
            else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "echo", 4) == 0)
                ft_echo(&shell);
            else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "env", 3) == 0)
                ft_env(&shell);
            else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "unset", 5) == 0)
                ft_unset(&shell);
            // else if (shell.cmd[0] != NULL && ft_strncmp(shell.cmd[0], "export", 6) == 0)
            //     ft_export(&shell);
            else
                ft_execute(shell.cmd, shell.env);
            ft_free_split(shell.cmd);
            free(shell.str);
        }
    }
    ft_free_split(shell.env);
    return(0);
}
