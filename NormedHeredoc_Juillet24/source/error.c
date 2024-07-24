/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 11:00:47 by marvin            #+#    #+#             */
/*   Updated: 2024/04/13 11:00:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error(t_shell *shell, int exit_code)
{
	perror("Error");
	safe_exit(shell, NULL, NONE, exit_code);
}

void	ft_no_path(t_shell *shell)
{
	printf("PATH not found\n");
	safe_exit(shell, NULL, NONE, 127);
}

void	exec_errors(t_shell *shell, t_command *cmd)
{
	if (errno == 13 && chdir(*cmd->v_cmd) == -1)
	{
		print_errors("minishell: ", cmd->v_cmd[0],
			": Permission denied\n");
		shell->exitcode = 126;
	}
	else if (errno == 13)
	{
		print_errors("minishell: ", cmd->v_cmd[0],
			": Is a directory\n");
		shell->exitcode = 126;
	}
	else if (**cmd->v_cmd == '/' || (**cmd->v_cmd == '.'
			&& **cmd->v_cmd + 1 == '/'))
	{
		print_errors("minishell: ", cmd->v_cmd[0],
			": No such file or directory\n");
		shell->exitcode = 127;
	}
	else
	{
		print_errors(NULL, cmd->v_cmd[0], ": command not found\n");
		shell->exitcode = 127;
	}
}
