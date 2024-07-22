/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:34:25 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/17 07:12:58 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	are_quotes_closed(const char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if ((str[i] == '"' && quotes == 0) || (str[i] == '\'' && quotes < 0))
			quotes++;
		else if ((str[i] == '\'' && quotes == 0)
			|| (str[i] == '"' && quotes > 0))
			quotes--;
		i++;
	}
	if (quotes == 0)
		return (false);
	return (true);
}

int	set_return_value(unsigned char return_value)
{
	static unsigned char	save = 0;
	static unsigned char	save2 = 0;

	save2 = save;
	save = return_value;
	return (save2);
}

bool	is_more_than_two_redirections(const char *str)
{
	int	i;
	int	count;

	i = 0;
	while (str[i])
	{
		count = 0;
		while (str[i] && (str[i] == '<' || str[i] == '>' || ft_isspace(str[i]))
			&& !is_in_quotes(str, i))
		{
			if ((str[i] == '<' || str[i] == '>') && !is_in_quotes(str, i))
				count++;
			i++;
		}
		if (count > 2)
			return (true);
		if (str[i])
			i++;
	}
	return (false);
}

bool	is_redirection_followed_by_pipe(const char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		if (str[i] == '|' && !is_in_quotes(str, i))
		{
			j = i - 1;
			while (str[j] && str[j] != '|' && !is_in_quotes(str, j))
			{
				if ((str[j] == '<' || str[j] == '>') && !is_in_quotes(str, j))
					return (true);
				if (!ft_isspace(str[j]))
					break ;
				j--;
			}
		}
		i++;
	}
	return (false);
}

bool	is_ending_by_redirection(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	i--;
	while (i >= 0 && str[i])
	{
		if (str[i] == '>' || str[i] == '<')
			return (true);
		if (!ft_isspace(str[i]))
			return (false);
		i--;
	}
	return (false);
}

bool	is_redirection_valid(const char *str)
{
	if (is_ending_by_redirection(str))
		return (false);
	if (is_redirection_followed_by_pipe(str))
		return (false);
	if (is_more_than_two_redirections(str))
		return (false);
	return (true);
}

bool	is_empty_pipe(const char *str)
{
	int		i;
	bool	empty;

	i = 0;
	empty = false;
	while (str[i] && empty == false)
	{
		while (str[i] && (str[i] != '|' || (str[i] == '|'
					&& is_in_quotes(str, i))))
			i++;
		if (!str[i])
			return (false);
		if (str[i + 1] == '|')
			return (true);
		i++;
		empty = true;
		while (str[i] && (str[i] != '|' || (str[i] == '|'
					&& is_in_quotes(str, i))))
		{
			if ((!ft_strchr("|<>", str[i]) && !ft_isspace(str[i])))
				empty = false;
			i++;
		}
	}
	return (empty);
}

bool	is_ended_by_pipe(const char *str)
{
	int		i;
	int		j;
	bool	pipe;

	i = 0;
	j = -1;
	pipe = true;
	while (str[i])
	{
		if (str[i] == '|' && !is_in_quotes(str, i))
			j = i + 1;
		i++;
	}
	while (j > -1 && str[j] && pipe == true)
	{
		if (!ft_isspace(str[j]))
			pipe = false;
		j++;
	}
	if (j == -1)
		pipe = false;
	return (pipe);
}

bool	is_starting_by_pipe(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '|')
	{
		if (str[i] != '|' && !ft_isspace(str[i]))
			return (false);
		i++;
	}
	if (!str[i])
		return (false);
	return (true);
}

bool	is_invalid_char_in_quote(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((ft_strchr("~`#&*()\\[]{};!", str[i]) && !is_in_quotes(str, i))
			|| (str[i] == '?' && !is_in_quotes(str, i)
				&& i > 0 && str[i - 1] != '$'))
			return (true);
		i++;
	}
	return (false);
}

bool	is_command_valid(const char *message)
{
	if (are_quotes_closed(message))
		ft_putstr_fd(ERR_QUOTE_NOT_CLOSED, 2);
	if (is_invalid_char_in_quote(message))
		ft_putstr_fd(ERR_SPECIAL_CHAR, 2);
	else if (is_starting_by_pipe(message))
		ft_putstr_fd(ERR_PIPE_START, 2);
	else if (is_ended_by_pipe(message))
		ft_putstr_fd(ERR_PIPE_END, 2);
	else if (is_empty_pipe(message))
		ft_putstr_fd(ERR_EMPTY_PIPE, 2);
	else if (!is_redirection_valid(message))
		ft_putstr_fd(ERR_INVALID_REDIRECTION, 2);
	else
		return (true);
	ft_free((char *)message);
	set_return_value(2);
	return (false);
}
//tous ce qui est avant a ete rajouté
/////////////////////////////////////////////////////////

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

	if (!is_command_valid(shell->message)) //TODO
		return (false);
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