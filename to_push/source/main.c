/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:49 by bchapuis          #+#    #+#             */
/*   Updated: 2024/04/18 13:59:04 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*alloc_join(char const *s1, char const *s2)
{
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	return (str);
}

char	*ft_strjoin_f(char *s1, char *s2, bool to_free)
{
	size_t		i;
	char		*str;

	i = 0;
	str = alloc_join(s1, s2);
	if (str == NULL)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		str[i] = *s2;
		s2++;
		i++;
	}
	if (to_free)
		free(s1);
	str[i] = '\0';
	return (str);
}

char	*path_string(t_shell *shell)
{
	char	*path;
	char	*path_end;
	char	*pwd;
	int		err;
	
	//pwd = getcwd(NULL, 0); // techniquement faux , il faut NOTRE pwd
	pwd = ft_get_cwd(shell, &err);


	path = ft_strjoin_f("minishell: ", pwd, false);
	//path = ft_strjoin("\033[1;34mminishell(\033[1;31m", pwd);
	free(pwd);
	//path_end = ft_strjoin(path, "\033[1;34m)> \033[0m");
	path_end = ft_strjoin_f(path, ")> ", false);
	//printf("\033[%zuC", ft_strlen(path_end));
	//printf("\033[s");
	//printf("\033[%zuD", ft_strlen(path_end));
	free(path);
	return (path_end);
}

char	*ft_read_input(t_shell *shell)
{
    char	*str;
	char	*path;

	path = path_string(shell);
    str = readline(path);
	free(path);
	if (str != NULL && str[0])
    	add_history(str);
    return(str);
}

size_t	ft_strlen_c(const char *str, char c, bool *found) //TO ADD
{
	size_t	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (str[i] == c)
		*(found) = true;
	return (i);
}

char	*ft_strdup_c(const char *s, int *j, char c, bool *found) //TO ADD
{
	size_t	len;
	char	*str;
	int		i;

	len = ft_strlen_c(s, c, found);
	str = malloc(sizeof(char) * len + 1); // MALLOC
	if (!str)
		return (NULL);
	i = 0;
	while (s[i] != c && s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	*j = ++i;
	return (str);
}

int	ft_strcmp_c( const char *first, const char *second, char c)
{
	size_t	i;

	i = 0;
	while (first[i] && second[i] && (first[i] == second[i]) 
		&& second[i] != c && first[i] != c)
		i++;
	if ((second[i] == '=' && !first[i]) || (first[i] == '=' && !second[i]))
		return (0);
	return ((unsigned char)first[i] - (unsigned char)second[i]);
}

int	ft_strcmp( const char *first, const char *second)
{
	size_t	i;

	i = 0;
	while (first[i] && second[i] && (first[i] == second[i]))
		i++;
	return ((unsigned char)first[i] - (unsigned char)second[i]);
}

int		del_envlist(t_shell **shell, char *name)
{
	t_env_list	*var;
	t_env_list	*new;

	var = (*shell)->list;
	if (ft_strcmp(name, var->var_name) == 0) // si la 1ere var est celle recherchee
	{
		(*shell)->list = var->next;
		free(var->var_name);
		free(var->value);
		free(var);
		return (0);
	}
	while (ft_strcmp(name, var->next->var_name) != 0)
	{
		var = var->next;
		if (var->next == NULL)
			return (1);
	}
	new = var->next->next;
	free(var->next->var_name);
	free(var->next->value);
	free(var->next);
	var->next = new;
	return (0);
}

void	add_envlist(t_shell **shell, t_env_list **new_var, char *env, bool lone)
{
	int		j;
	bool	found;

	j = 0;
	found = false;
	if (lone == false)
	{
		(*new_var) = malloc(sizeof(t_env_list)); // a check
		(*new_var)->var_name = ft_strdup_c(env, &j, '=', &found);
		env += j;
		if (found == false)
			(*new_var)->value = NULL;
		else
			(*new_var)->value = ft_strdup_c(env, &j, '\0', &found);
		(*new_var)->next = (*shell)->list;
		(*shell)->list = (*new_var);
	}
	else
	{
		(*shell)->list = malloc(sizeof(t_env_list)); // a check
		(*shell)->list->var_name = ft_strdup_c(env, &j, '=', &found);
		env += j;
		if (found == false)
			(*shell)->list->value = NULL;
		else
			(*shell)->list->value = ft_strdup_c(env, &j, '\0', &found);
		(*shell)->list->next = NULL;
	}
}

void init_envp(t_shell *shell, char **envp)
{
	t_env_list	*new_var;
    size_t		len = 0;
    size_t		i = 0;

    while (envp[len])
		len++;
    shell->env = malloc((len + 1) * sizeof(char *)); // malloc du char**
	
	while (i < len)
    {
        shell->env[i] = ft_strdup(envp[i]); //remplissage char **
        i++;
    }
	i = 0; // 0 et pas -1 car la premiere var est deja mise juste la : 
	add_envlist(&shell, &new_var, envp[0], true); // ajout tete de la liste chainee
    while (++i < len)
		add_envlist(&shell, &new_var, envp[i], false); // rempli liste
    shell->env[len] = NULL;
}

char **ft_parsing(t_shell *shell)
{
    char **cmd;
    cmd = ft_split(shell->str, ' ');
    if (cmd == NULL)
        exit(0);
    return (cmd);
}

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
	set_signals();
    init_envp(&shell, envp);
    while(shell.exit == 0)
    {
        shell.str = ft_read_input(&shell);
        if (shell.str != NULL)
        {
            
            shell.cmd = ft_parsing(&shell);
            //shell.commande = lexer(shell.str);
            //print_tokens(shell.commande);
			//free_tokens(shell.commande);

			// NOTE : j'ai mis tout les strncmp en strcmp ->
            if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "exit") == 0)
                shell.exit = ft_is_exit(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "cd") == 0)
                ft_cd(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "pwd") == 0)
                ft_pwd(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "echo") == 0)
                ft_echo(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "env") == 0)
                ft_env(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "unset") == 0)
                ft_unset(&shell);
            else if (shell.cmd[0] != NULL && ft_strcmp(shell.cmd[0], "export") == 0)
                ft_export(&shell);
            else
                ft_execute(shell.cmd, shell.env, &shell);
            ft_free_split(shell.cmd);
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
