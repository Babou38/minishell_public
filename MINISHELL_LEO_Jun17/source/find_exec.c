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

void	child_process(char **cmd, char **envp) //N
{
	char	*path;
	
	path = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	printf("child is running ... better go catch it ..\n");
	// printf("cmd : (%p)\n", cmd);
	// printf("envp : (%p)\n", envp);
    path = get_cmd_path(cmd[0], envp);
	// printf("path : (%p)\n", path);
    if (path == NULL || execve(path, cmd, envp) == -1)
    {
    	printf("%s: command not found\n", cmd[0]);
    	free(path);
    	exit(127); // il faut aussi free tout le reste: 
		// en cas de dup de minishell, on doit free history,
		// arbre, path ...... -> fonction ? 
    }
}

void	analyse_status(t_shell *shell, int status)
{
	int sig;

	if (WIFEXITED(status))
		shell->exitcode = WIFEXITED(status);
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
		child_process(cmd, envp);
    waitpid(pid, &status, 0);
	analyse_status(shell, status);
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
                error(NULL, 1);
        }
        full_path = ft_strjoin(tmp, cmd);
        if (full_path == NULL)
        {
                free(tmp);
                free(path);
                error(NULL, 1);
        }
        free(tmp);
        return (full_path);
}

char    *check_path(char **path, char *cmd)
{
        int             i;
        char    *full_path;

        i = -1;
		// printf("---------checkpath---------\n");
		// printf("cmd : %p\n", cmd);
		// printf("path : %p\n", path);
		// printf("------------------\n");
        while (path[++i])
        {
                full_path = join_path_cmd(path[i], cmd);
				// printf("full path : %p\n", full_path);
				// printf("full path str: %s\n", full_path);
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
                        error(NULL, 126);
        }
        while (env[++i])
                if (!ft_strncmp(env[i], "PATH=", 5))
                        break ;
        if (env[i] == NULL)
                ft_no_path();
        path = ft_split(env[i] + 5, ':');
		// printf("path str: %s\n", env[i]);
        if (path == NULL)
                error(NULL, 1);
        return (check_path(path, cmd));
}