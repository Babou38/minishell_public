/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:49 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/21 15:24:09 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signal_handler_c(int signum)
{
	(void)signum;
	printf("\n");
	rl_replace_line("\0", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler_c);
}

void	get_and_exec(t_shell *shell)
{
	if (process_message(shell, shell->str))
		safe_exit(shell, NULL, NONE, 1);
	pipes_commands(shell, shell->cmd_list, 0);
	free_heredocs(shell->hdoc_list);
	shell->hdoc_list = NULL;
	ft_free(shell->heredoc_name);
	shell->heredoc_name = NULL;
	free_cmd_list(shell->cmd_list);
	ft_free(shell->message);
}

void	dup_and_env(t_shell *shell, char **envp)
{
	shell->cmd_list->cmd = NULL;
	shell->cmd_list->v_cmd = NULL;
	shell->cmd_list->heredoc_fd = 0;
	shell->cmd_list->input_redirection = NULL;
	shell->cmd_list->output_redirection = NULL;
	shell->cmd_list->next = NULL;
	shell->cmd_list->prev = NULL;
	init_envp(shell, envp);
	shell->stdin = dup(STDIN_FILENO);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_command	c_list;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.cmd_list = &c_list;
	dup_and_env(&shell, envp);
	while (shell.exit == 0)
	{
		set_signals();
		shell.str = ft_read_input(&shell);
		if (shell.str && shell.str[0] == '\0')
		{
			free(shell.str);
			continue ;
		}
		if (shell.str != NULL)
			get_and_exec(&shell);
		else
			exit_ctrl(&shell, 0);
	}
	safe_exit(&shell, NULL, END, shell.exitcode);
	return (0);
}
