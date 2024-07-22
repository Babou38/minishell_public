/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:41:50 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/21 23:01:26 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	input_redirection(t_shell *shell, t_command *cmd, int pipe_fd, int i)
{
	int		fd;
	char	*path;

	(void)pipe_fd;
	path = next_redirection_name(cmd, i);
	if (!path)
	{
		perror("minishell: malloc: ");
		exit(2);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (cmd->next)
			safe_exit(shell, path, STR, 0);
		safe_exit(shell, path, STR, 1);
	}
	if (!ft_strncmp(path, cmd->input_redirection, ft_strlen(path))
		&& check_last_redirection(cmd->cmd + i, '<'))
		if (dup2(fd, STDIN_FILENO) < 0)
			safe_exit(shell, path, STR, 1);
	free(path);
	close(fd);
}

// void	heredoc_redirection(t_shell *shell, t_command *cmd, int i)
// {
//     char	*eof;

// 	eof = next_redirection_name(cmd, i);
// 	if (!eof)
// 		safe_exit(shell, eof, STR, 2);
//     printf("Processing heredoc redirection for EOF: %s\n", eof); // Debug message
//     printf("Current heredoc name: %s\n", shell->heredoc_name); // Debug message
// 	if (!ft_strncmp(eof, cmd->input_redirection, ft_strlen(eof)))
// 	{
// 		cmd->heredoc_fd = open(shell->heredoc_name,O_RDONLY, 0666);
// 		if (cmd->heredoc_fd < 0)
// 			perror("minishell: heredocument");
//         printf("Opened heredoc file descriptor: %d\n", cmd->heredoc_fd); // Debug message
// 		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
// 			safe_exit(shell, eof, STR, 1);
//         printf("Duplicated heredoc file descriptor to STDIN\n"); // Debug message
// 	}

//     // Only close heredoc_fd if it's valid
//     if (cmd->heredoc_fd >= 0)
//     {
//         close(cmd->heredoc_fd);
//         cmd->heredoc_fd = -1; // Invalidate the descriptor to prevent reuse
//         printf("Closed heredoc file descriptor\n"); // Debug message
//     }

// 	free(eof);
// 	// close(cmd->heredoc_fd);

//     //  char	*eof;

// 	// eof = next_redirection_name(cmd, i);
// 	// if (!eof)
// 	// 	safe_exit(shell, eof, STR, 2);
// 	// if (!ft_strncmp(eof, cmd->input_redirection, ft_strlen(eof)))
// 	// {
// 	// 	cmd->heredoc_fd = open(".tmp.txt",O_RDONLY, 0666);
// 	// 	if (cmd->heredoc_fd < 0)
// 	// 		perror("minishell: heredocument");
// 	// 	if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
// 	// 		safe_exit(shell, eof, STR, 1);
// 	// }
// 	// free(eof);
// 	// close(cmd->heredoc_fd);
// }

void heredoc_redirection(t_shell *shell, t_command *cmd, int i)
{
    char *eof;

    eof = next_redirection_name(cmd, i);
    if (!eof)
        safe_exit(shell, eof, STR, 2);
    t_heredoc *current_heredoc = shell->heredocs;
    while (current_heredoc)
    {
        if (ft_strcmp(current_heredoc->eof, eof) == 0)
        {
            cmd->heredoc_fd = open(current_heredoc->filename, O_RDONLY);
            if (cmd->heredoc_fd < 0)
            {
                perror("minishell: heredocument");
                free(eof);
                return;
            }
            if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
            {
                perror("dup2 failed");
                safe_exit(shell, eof, STR, 1);
            }
            close(cmd->heredoc_fd);
            break;
        }
        current_heredoc = current_heredoc->next;
    }
    free(eof);
}


static void	trunc_redirection(t_shell *shell, t_command *cmd, int pipe_fd, int i)
{
	int		fd;
	char	*path;

	path = next_redirection_name(cmd, i);
	if (!path)
	{
		perror("minishell: malloc: ");
		exit(2);
	}
	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		if (cmd->next)
			safe_exit(shell, path, STR, 0);
		safe_exit(shell, path, STR, 1);
	}
	if (!ft_strncmp(path, cmd->output_redirection, ft_strlen(path))
		&& check_last_redirection(cmd->cmd + i, '>'))
		if (dup2(fd, pipe_fd) < 0)
			safe_exit(shell, path, STR, 1);
	free(path);
	close(fd);
}

static void	append_redirection(t_shell *shell, t_command *cmd, int pipe_fd, int i)
{
	int		fd;
	char	*path;

	path = next_redirection_name(cmd, i);
	if (!path)
	{
		perror("minishell: malloc: ");
		exit(2);
	}
	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);	
		perror(path);
		safe_exit(shell, path, STR, 1);
	}
	if (!ft_strncmp(path, cmd->output_redirection, ft_strlen(path)) // ? pourquoi ? 
		&& check_last_redirection(cmd->cmd + i, '>'))
		if (dup2(fd, pipe_fd) < 0)
			safe_exit(shell, path, STR, 1);
	free(path);
	close(fd);
}

void	in_out_redirection(t_shell *shell, t_command *command, int pipe_fd, int i)
{
	if (command->cmd[i] == '>' || command->cmd[i] == '<')
		i++;
	if (command->cmd[i - 1] == '>' && command->cmd[i] == '>')
		append_redirection(shell, command, pipe_fd, i + 1);
	else if (command->cmd[i - 1] == '>')
		trunc_redirection(shell, command, pipe_fd, i);
	else if (command->cmd[i - 1] == '<' && command->cmd[i] == '<')
		heredoc_redirection(shell, command, i);
	else if (command->cmd[i - 1] == '<')
		input_redirection(shell, command, pipe_fd, i);
}

char	*redirection(t_command *cmd, char input_token, int i)
{
	char	*str;
	int		start;

	if (!is_there_chr(cmd->cmd, input_token))
		return (NULL);
	str = malloc(sizeof(char)
			* (last_redirection_size(cmd->cmd, input_token, &start) + 1)); //size a malloc pour la redir
	if (!str)
	{
		perror("minishell: malloc: ");
		return (NULL);
	}
	while (cmd->cmd[start]) //recopie la dir
	{
		if (ft_isspace(cmd->cmd[start]) && !is_in_quotes(cmd->cmd, start))
			break ;
		str[i++] = cmd->cmd[start++];
	}
	str[i] = '\0';
	if (is_there_chr(str, '"') || is_there_chr(str, '\'')) //enleve les ""
		str = str_without_quotes(str);
	return (str);
	
}

void	exec_redirections(t_shell *shell, t_command *cmd, int i)
{
	if (!is_there_chr(cmd->cmd, '>') && !is_there_chr(cmd->cmd, '<')) //Si pas de redir, on quitte
		return ;
	cmd->input_redirection = redirection(cmd, '<', 0); //quelle redir est la plus a droite ?
	cmd->output_redirection = redirection(cmd, '>', 0);
	while (cmd->cmd[i])
	{
		if (cmd->cmd[i] == '>' && !is_in_quotes(cmd->cmd, i))
			in_out_redirection(shell, cmd, STDOUT_FILENO, i++);
		if (cmd->cmd[i] == '<' && !is_in_quotes(cmd->cmd, i))
			in_out_redirection(shell, cmd, STDIN_FILENO, i++);
		i++;
	}
}