/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:49 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/26 15:12:28 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signal_handler_c(int signum)
{
	// if (signum == SIGQUIT)
	// {
	// 	rl_replace_line("\0", 0);
	// 	rl_on_new_line();
	// 	rl_redisplay();
	// 	printf("\033[u");
	// 	printf("\033[K");
	// 	printf("\033[u");
	// }
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("\0", 0);
		rl_on_new_line();
		rl_redisplay();
		//printf("\033[s");
		//rl_ding(); 
	}
}

void	set_signals(void)
{
	struct sigaction	action;

	action.sa_handler = signal_handler_c;
	sigemptyset(&(action.sa_mask));
	signal(SIGQUIT, SIG_IGN);
	sigaddset(&(action.sa_mask), SIGINT);
	action.sa_flags = 0;
	if (sigaction(SIGINT, &action, NULL) == -1)
		printf("SIGACTION ERROR!\n");
}

int main (int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    shell.exit = 0;
    shell.exitcode = 0;
	shell.heredoc_counter = 0;
    set_signals();
    init_envp(&shell, envp);
    while(shell.exit == 0)
    {
        shell.str = ft_read_input(&shell);
        if (shell.str != NULL)
        {
            shell.list_token = lexer(shell.str);
            // print_tokens(shell.list_token);
            shell.ast = build_ast(shell.list_token);
            // printAST(shell.ast);
            execute_ast(shell.ast, &shell);
            free_ast(shell.ast);
            free_tokens(shell.list_token);
            free(shell.str);
        }
        else
		{
			printf("exit\n");
			rl_clear_history();
			shell.exit = 1;
		}
    }
    ft_free_split(shell.env);
    ft_free_env(&shell.list);
    return(0);
}
