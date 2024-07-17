/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:18:58 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/17 06:36:26 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool handle_command3(t_shell *shell, t_command *cmd)
{
    if (cmd->v_cmd[0] != NULL && ft_strlen(cmd->v_cmd[0]) > 0)
    {
        if (ft_strcmp(cmd->v_cmd[0], "exit") == 0)
            shell->exit = ft_is_exit(shell, cmd->v_cmd);
        else if (ft_strcmp(cmd->v_cmd[0], "cd") == 0)
            ft_cd(shell, cmd->v_cmd); 
        else if (ft_strcmp(cmd->v_cmd[0], "pwd") == 0)
            ft_pwd(shell);
        else if (ft_strcmp(cmd->v_cmd[0], "echo") == 0)
            ft_echo(shell, cmd->v_cmd);
        else if (ft_strcmp(cmd->v_cmd[0], "env") == 0 )
            ft_env(shell);
        else if (ft_strcmp(cmd->v_cmd[0], "unset") == 0)
            ft_unset(shell, cmd->v_cmd);
        else if (ft_strcmp(cmd->v_cmd[0], "export") == 0)
            ft_export(shell, cmd->v_cmd);
		else
			return (false);
		return (true);
    }
	else
		return (false);
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

int	exec(t_shell *shell, t_command *cmd, int i)
{
	char	*path;
	// int		test;

	(void)i; // to clean q 	
	// test = 2;
	path = NULL;
	if (handle_command3(shell, cmd))
		return (0);
	path = get_cmd_path(shell, cmd->v_cmd[0], shell->env);
	if (path == NULL)
    {
    	printferr(" command not found\n");
    	safe_exit(shell, NULL, NONE, 127);
    }
    execve(path, cmd->v_cmd, shell->env);
	perror(": ");
    safe_exit(shell, NULL, NONE, 1);
	return (1);
}
