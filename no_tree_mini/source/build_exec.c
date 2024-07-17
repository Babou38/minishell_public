/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:34:25 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/08 12:34:31 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*cut_commands(const char *message, int letter, int j)
{
	static int	i = 0;
	char		*cmd;

	if (message[i] == '|')
		i++;
	j = i;
	while (message[i])
	{
		if (message[i] == '|' && !is_in_quotes(message, i))
			break ;
		i++;
	}
	cmd = malloc(sizeof(char) * (i - j + 1));
	if (!cmd)
		return (NULL);
	while (j < i)
		cmd[letter++] = message[j++];
	cmd[letter] = '\0';
	if (!message[i])
		i = 0;
	return (cmd);
}

void	cmd_new(t_command *prev)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	if (!command)
		return ;
	ft_memset(command, 0, sizeof(t_command));
	prev->next = command;
	command->prev = prev;
}

bool	create_cmd_list(t_shell *shell)
{
	t_command	*command;
	size_t		i;

	i = 0;
	command = shell->cmd_list;
	while (i < shell->nb_pipes)
	{
		cmd_new(command);
		if (!command->next)
		{
			ft_free(shell->message);
			free_cmd_list(command);
			perror("minishell: malloc: ");
			return (false);
		}
		command = command->next;
		i++;
	}
	return (true);
}

bool	split_command(t_shell *shell, t_command *command)
{
	char	*cmd_clean;

	while (command)
	{
		cmd_clean = clean_command(command->cmd); //
		if (!cmd_clean)
		{
			ft_free(shell->message);
			free_cmd_list(command);
			perror("minishell: malloc: ");
			return (false);
		}
		command->v_cmd = split_arguments(cmd_clean, " \n\r\t\v\f"); //tab ** des commandes sans redir
		if (!command->v_cmd)
		{
			ft_free(shell->message);
			ft_free(cmd_clean);
			free_cmd_list(command);
			perror("minishell: malloc: ");
			return (false);
		}
		command = command->next;
		ft_free(cmd_clean);
	}
	return (true);
}

bool	standardize_command(t_shell *shell, t_command *command)
{
	command = shell->cmd_list; //(norme)

	// if (!is_command_valid(shell->message)) //TODO
	// 	return (false);

	shell->nb_pipes = count_pipes(shell->message);
	if (!create_cmd_list(shell))
		return (false);
	while (command)
	{
		// command->cmd = ft_free_strtrim(cut_commands(shell->message, 0, 0), //separe en morceaux autour des pipes
		// 		" \n\r\t\v\f"); //free trim eneleve les espaces restants
		command->cmd = cut_commands(shell->message, 0, 0);
		if (!command->cmd)
		{
			ft_free(shell->message);
			free_cmd_list(shell->cmd_list);
			perror("minishell: malloc: ");
			return (false);
		}
		command = command->next;
	}
	if (!split_command(shell, shell->cmd_list))
		return (false);
	// command = shell->cmd_list;
	// int k;
	// while (command)
	// {
	// 	k = 0;
	// 	printf("\n\nCMD : \n\n");
	// 	while (command->v_cmd[k])
	// 	{
	// 		printf("cmd clean: %s\n", command->v_cmd[k]);
	// 		k++;
	// 	}
	// 	command = command->next;
	// }
	return (true);
}

int	process_message(t_shell *shell, char *message)
{

	shell->message = message;
	ft_memset2(shell->cmd_list, 0, sizeof(t_command));
	if (!standardize_command(shell, NULL))
		return (4);
	// pipes_commands(shell, shell->cmd_list, 0);
	// if (access(".tmp.txt", 1))
	// 	unlink(".tmp.txt");
	// free_cmd_list(shell->cmd_list);
	// ft_free(shell->message);
	return (0);
}