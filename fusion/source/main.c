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

#include "minishell.h"

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

// char *ft_read_input(void)
// {
//     char *pwd;
//     char *str;

//     pwd = getcwd(NULL, 0);
//     printf("\033[1;34mminishell(\033[0m\033[1;31m%s\033[0m", pwd);
//     str = readline("\033[1;34m)> \033[0m");
//     free(pwd);
//     add_history(str);
//     return(str);
// }

// static char *strjoin_and_free(char *s1, const char *s2)
// {
//     char *new_str;
//     char *temp_str;

//     if (!s1 || !s2)
//         return (NULL);
//     temp_str = ft_strjoin(s1, " ");
//     if (!temp_str)
//         return (NULL);
//     new_str = ft_strjoin(temp_str, s2);
//     free(temp_str);
//     free(s1);
//     return (new_str);
// }

// char *ft_read_input(void)
// {
//     char *pwd;
//     char *str;
//     char *input;
//     char *temp_input;

//     pwd = getcwd(NULL, 0);
//     printf("\033[1;34mminishell(\033[0m\033[1;31m%s\033[0m", pwd);
//     str = readline("\033[1;34m)> \033[0m");
//     free(pwd);
//     add_history(str);
//     input = str;
//     while (input[ft_strlen(input) - 1] == '|')
//     {
//         temp_input = NULL;
//         while (!temp_input || !*temp_input)
//         {
//             if (temp_input)
//                 free(temp_input);
//             temp_input = readline("> ");
//         }
//         input = strjoin_and_free(input, temp_input);
//         free(temp_input);
//         if (!input)
//             return (NULL);
//     }
//     return (input);
// }

void printAST(t_ast_node* node)
{
    if (node == NULL)
        return;
    printf("Type: %d ", node->type);
    printf("Command: %s \n", node->value);
    if (node->type == TOKEN_CMD)
    {
        if (node->args != NULL)
        {
            int i = 0;
            while (node->args[i] != NULL)
            {
                printf("Args: %s ", node->args[i]);
                i++;
            }
        }

    }
    printf ("\n");
    printAST(node->left);
    printAST(node->right);
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
