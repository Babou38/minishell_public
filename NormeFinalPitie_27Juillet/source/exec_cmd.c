/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:18:58 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/16 15:18:59 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	handle_command3(t_shell *shell, t_command *cmd)
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
		else if (ft_strcmp(cmd->v_cmd[0], "env") == 0)
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

void	path_is_real(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			break ;
		i++;
	}
	if (env[i] == NULL)
		ft_no_path(shell);
}

char	**get_split_path(t_shell *shell, char **env)
{
	char	**path;
	int		i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			break ;
	}
	path = ft_split(env[i] + 5, ':');
	if (path == NULL)
		error(shell, 2);
	return (path);
}

int	final_exec_cmd(t_shell *shell, t_command *cmd, char *path, int i)
{
	if (!path)
		path = get_cmd_path(shell, cmd->v_cmd[0], shell->split_path, i);
	if (!path)
	{
		ft_free_split(shell->split_path);
		safe_exit(shell, NULL, NONE, 2);
	}
	if (execve(path, cmd->v_cmd, shell->env) == -1 && errno != 2)
	{
		ft_free_split(shell->split_path);
		shell->split_path = NULL;
		ft_free(path);
		return (1);
	}
	return (0);
}

int	exec(t_shell *shell, t_command *cmd)
{
	char	*path;
	int		i;
	int		flag;

	path = NULL;
	flag = 0;
	i = 0;
	if (handle_command3(shell, cmd))
		return (0);
	path_is_real(shell, shell->env);
	path = manage_path_type(shell, cmd->v_cmd[0], &flag);
	shell->split_path = get_split_path(shell, shell->env);
	while (shell->split_path && shell->split_path[i])
	{
		if (final_exec_cmd(shell, cmd, path, i))
			return (1);
		ft_free(path);
		path = NULL;
		i++;
	}
	return (1);
}
