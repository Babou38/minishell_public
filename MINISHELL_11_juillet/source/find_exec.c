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

#include "../includes/minishell.h"

void	child_process(t_shell *shell, char **cmd, char **envp) //N
{
	char	*path;
	struct stat statbuf;
	
	path = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	// printf("child is running ... better go catch it ..\n");
    path = get_cmd_path(shell, cmd[0], envp);
	if (path == NULL)
    {
    	printferr(" command not found\n");
    	safe_exit(shell, NULL, NONE, 127);
    }
	if(stat(path, &statbuf) == -1){
      perror("stat");
      safe_exit(shell, path, STR, 1);
    }
	if (S_ISDIR(statbuf.st_mode))
	{
    	printferr(" Is a directory\n");
    	safe_exit(shell, NULL, NONE, 126);
    }
    if (execve(path, cmd, envp) == -1)
    {
		perror(": ");
    	// printf("%s: command not found\n", cmd[0]);
    	safe_exit(shell, NULL, NONE, 1);
    }
}

void	analyse_status(t_shell *shell, int status)
{
	int sig;

	if (WIFEXITED(status))
	{
		shell->exitcode = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == 3) //SIGQUIT
			printf("Quit (core dumped)\n");
		if (sig == 2) // SIGINT
			printf("\n");
		shell->exitcode = 128 + sig;
	}
}

void ft_execute(char **cmd, char **envp, t_shell *shell)
{
	pid_t   pid;
    int     status;

    if (!cmd || cmd[0] == NULL)
 		return ;
	// set_signals_interrupt();  // Grossier, moche, insulte a la programmation, mais fonctionne pour le moment
	pid = fork();
    if (pid == -1)
        perror("fork failed");
	else if (pid > 0)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
    else if (pid == 0)
		child_process(shell, cmd, envp);
    waitpid(pid, &status, 0);
	analyse_status(shell, status);
}

char    *join_path_cmd(t_shell *shell, char *path, char *cmd)
{
        char    *tmp;
        char    *full_path;

        tmp = ft_strjoin(path, "/");
        if (tmp == NULL)
        {
                free(path);
                error(shell, 1);
        }
        full_path = ft_strjoin(tmp, cmd);
        if (full_path == NULL)
        {
                free(tmp);
                free(path);
                error(shell, 1);
        }
        free(tmp);
        return (full_path);
}

char    *check_path(t_shell *shell, char **path, char *cmd)
{
        int             i;
        char    *full_path;

        i = -1;
        while (path[++i])
        {
                full_path = join_path_cmd(shell, path[i], cmd);
                if (access(full_path, X_OK) == 0)
                        return (ft_free_split(path), full_path);
                free(full_path);
        }
        ft_free_split(path);
        return (NULL);
}

char    *get_cmd_path(t_shell *shell, char *cmd, char **env)
{
        int             i;
        char    **path;

        i = -1;
        if (access(cmd, X_OK) == 0)
                return (cmd);
        else if (cmd[0] == '.' && cmd[1] == '/')
        {
                cmd += 2;
                if (access(cmd, X_OK) == 0)
                        return (cmd);
                else
                        error(shell, 126);
        }
        while (env[++i])
                if (!ft_strncmp(env[i], "PATH=", 5))
                        break ;
        if (env[i] == NULL)
                ft_no_path(shell);
        path = ft_split(env[i] + 5, ':');
        if (path == NULL)
                error(shell, 1);
        return (check_path(shell, path, cmd));
}