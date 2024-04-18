/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:29:42 by lchapard          #+#    #+#             */
/*   Updated: 2024/04/09 13:29:43 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char*	res;
//void	set_signals(void);

void	safe_leave(bool print)
{
	if (print)
		printf("exit\n");
	free(res);
	res = NULL;
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t		i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char )s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	print_split(char **m_split, int argc)
{
	int i;

	i = -1;
	while (++i < argc)
		printf("split %d : %s \n", i, m_split[i]);
	printf("nb words : %d\n", argc);

}

void	env_var(char *str)
{
	str++;
	printf("(%s)\n", getenv(str));
}

bool	end_quote(char *str, int *i, char c)
{
	if (str[*i] == c)
		(*i)++;
	while(str[*i] != c)
	{
		if (!str[*i])
		{
			//printf("could not find ...\n");
			return (true);
		}
		(*i)++;
	}
	//printf("found it!\n");
	return (false);
}

bool	check_quotes(char *str)
{
	int		i;
	bool	res;

	i = 0;
	res = false;
	if (str == NULL)
		return (false);
	while (str[i] && str[i] != '\'' && str[i] != '\"')
		i++;
	if (!str[i])
	{
		//printf("no quotes found !\n");
		return (false);
	}
	// printf("char 1= (%c)\n\n", str[i]);
	// printf("char 2= (%c)\n\n", str[i + 1]);
	while (str[i])
	{
		res = end_quote(str, &i, str[i]);
		if (res == true)
			return (true);
		i++;
		//printf("found it ! i'm at : %c\n", str[i]);
		while (str[i] && str[i] != '\'' && str[i] != '\"')
			i++;
	}
	return (res);
}

void	read_res()
{
	if (res == NULL)
	{
		printf("exit\n");
		if (res != NULL)
			free(res);
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	if (ft_strcmp("exit", res) == 0)
		safe_leave(true);
	if (ft_strcmp("clearh", res) == 0)
		rl_clear_history();
	if (res[0] == '$')
		env_var(res);
	if (res[0])
		add_history(res);

	// int ret;
	// ret = check_quotes(res);
	// if (ret == 0)
	// 	printf("quote closed : %d\n", ret);
	// else
	// 	printf("open quote ! : %d\n", ret);
}

void    get_termline()
{ 
	// char	**m_split;
	// int		argc;

	// m_split = NULL;
	while (1)
	{	
		res = readline("Enter a command : ");
		if (!check_quotes(res))
			read_res();
		else
		printf("Missing quotes !\n");
		//m_split = ft_split(res, ' ', &argc);
		//print_split(m_split, argc);
    	free(res);
		res = NULL;
	}
}

void    get_cwd()
{
	res = getcwd(NULL, 0);
	printf("cwd is : (%s)\n", res);
	free(res);
	res = NULL;
}

void	signal_handler_c(int signum)
{
	if (signum == SIGQUIT)
	{
		// printf("\n");
		// rl_replace_line("\0", 0);
		// rl_on_new_line();
		// rl_redisplay();
		//rl_redisplay();
		//sigaction = SIG_IGN to cancel pending
	}
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("\0", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	set_signals(void)
{
	struct sigaction	action;

	action.sa_handler = signal_handler_c;
	sigemptyset(&(action.sa_mask));
	sigaddset(&(action.sa_mask), SIGINT);
	action.sa_flags = 0;
	if (sigaction(SIGQUIT, &action, NULL) == -1)
		printf("SIGACTION ERROR!\n");
	if (sigaction(SIGINT, &action, NULL) == -1)
		printf("SIGACTION ERROR!\n");
}

int main(int argc, char **argv)
{
	(void)argv;
	set_signals();
	
	while(1)
	{
		get_termline();
	}
    return (argc);
}