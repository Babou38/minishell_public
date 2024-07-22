/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:58:47 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/08 15:58:48 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

bool handle_command2(t_shell *shell, t_command *cmd)
{
    if (cmd->v_cmd[0] != NULL && ft_strlen(cmd->v_cmd[0]) > 0)
    {
        if (ft_strcmp(cmd->v_cmd[0], "exit") == 0 && shell->nb_pipes == 0)
            shell->exit = ft_is_exit(shell, cmd->v_cmd);
        else if (ft_strcmp(cmd->v_cmd[0], "cd") == 0 && shell->nb_pipes == 0)
            ft_cd(shell, cmd->v_cmd); 
        else if (ft_strcmp(cmd->v_cmd[0], "pwd") == 0 && shell->nb_pipes == 0)
            ft_pwd(shell);
        else if (ft_strcmp(cmd->v_cmd[0], "env") == 0 && shell->nb_pipes == 0 )
            ft_env(shell);
        else if (ft_strcmp(cmd->v_cmd[0], "unset") == 0 && shell->nb_pipes == 0)
            ft_unset(shell, cmd->v_cmd);
        else if (ft_strcmp(cmd->v_cmd[0], "export") == 0 && shell->nb_pipes == 0)
            ft_export(shell, cmd->v_cmd);
		else
			return (false);
		return (true);
    }
	else
		return (false);
}

static void	open_pipes(t_shell *shell, unsigned int nb_pipes, int *pipe_fds)
{
	unsigned int	i;
	unsigned int	tmp;

	i = 0;
	while (i < nb_pipes)
	{
		if (pipe(pipe_fds + (2 * i)) < 0)
		{
			perror("Pipe opening error:\n");
			tmp = 0;
			while (tmp < i)
				close(pipe_fds[tmp++]);
			// exit(free_all(shell, NULL, EXIT_FAILURE));
			safe_exit(shell, NULL, NONE, 1);
		}
		i++;
	}
}

static void	wait_parent(t_shell *shell)
{
	int				sig;
	int				status;
	unsigned int	i;

	status = 0;
	i = 0;
	while (i < 2 * shell->nb_pipes)
		close(shell->pipe_fds[i++]);
	while (waitpid(0, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			shell->exitcode = (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == 3)
				write(2, "Quit (core dumped)\n", 19);
			if (sig == 2)
				printf("\n");
			if (sig == 2 || sig == 3)
				shell->exitcode = (sig + 128);
		}
	}
}

static void	copy_quoteless(char *result, char *str)
{
	unsigned int	i;
	unsigned int	j;
	int				qts;

	i = 0;
	j = 0;
	qts = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && qts == 0)
		{
			qts++;
			if (str[i] == '\'')
				qts -= 2;
		}
		else if ((str[i] == '\'' && qts == -1) || (str[i] == '"' && qts == 1))
			qts = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
}

char	*str_without_quotes(char *str)
{
	char			*result;

	//alloc taille sans les quotes
	result = malloc(sizeof(char) * (ft_strlen(str) - count_quotes(str) + 1));
	if (!result) 
	{
		perror("minishell: malloc: ");
		free(str);
		return (NULL);
	}
	copy_quoteless(result, str); //recopie sans quote
	free(str);
	return (result);
}

static void	child_exec(t_shell *shell, t_command *cmd, int *pipe_fds,
						unsigned int i)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->next) // un suivant ? -> ecrit dans pipe
		if (dup2(pipe_fds[i + 1], STDOUT_FILENO) < 0)
			safe_exit(shell, NULL, NONE, 1);
	if (i != 0) // un precedent ? lit dans pipe
		if (dup2(pipe_fds[i - 2], STDIN_FILENO) < 0)
			safe_exit(shell, NULL, NONE, 1);
	exec_redirections(shell, cmd, 0); // en le mettant apres , redirection en priorite sur les pipes (peut reecraser)
	i = 0;
	while (i < 2 * shell->nb_pipes) // 2 a close par nb_pipes donc i*2
		close(pipe_fds[i++]);
	close(shell->stdin);
	if (exec(shell, cmd, 0) == 1 && *cmd->v_cmd)
		exec_errors(shell, cmd);
	safe_exit(shell, NULL, NONE, 0);
}

void	child(t_shell *shell, t_command *cmd, int *pipe_fds, unsigned int i)
{
	int	pid;
	pid = fork();
	if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (pid == 0)
		child_exec(shell, cmd, pipe_fds, i);
	else if (pid < 0)
		safe_exit(shell, NULL, NONE, 1);
}

void	pipes_commands(t_shell *shell, t_command *command,
						unsigned int i)
{
	shell->pipe_fds = NULL;
	if (shell->nb_pipes)
	{
		shell->pipe_fds = malloc(sizeof(int) * (shell->nb_pipes * 2));
		if (!shell->pipe_fds)
		{
			perror("minishell: malloc: ");
			return ;
		}
		open_pipes(shell, shell->nb_pipes, shell->pipe_fds);
	}
	while (command)
	{
		shell->exitcode = 0;
		// if (here_document(shell, command))
		// 	break ;
		if (!handle_command2(shell, command))
			child(shell, command, shell->pipe_fds, i);
		dup2(shell->stdin, STDIN_FILENO); //pas essentiel ? pour heredoc surtout 
		command = command->next;
		i += 2;
	}
	wait_parent(shell);
	ft_free(shell->pipe_fds);
}