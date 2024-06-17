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

//-----------------------------------------
// char	*path_string(t_shell *shell)
// {
// 	char	*path;
// 	char	*path_end;
// 	char	*pwd;
// 	int		err;
	
// 	pwd = ft_get_cwd(shell, &err);
// 	//path = ft_strjoin_f("minishell: ", pwd, false);
// 	path = ft_strjoin("\001\033[1;34m\002minishell(\001\033[1;31m\002", pwd);
// 	free(pwd);
// 	path_end = ft_strjoin(path, "\001\033[1;34m\002)> \001\033[0m\002");
// 	//path_end = ft_strjoin_f(path, "> ", false);
// 	free(path);
// 	return (path_end);
// }
//----------------------------------------

int	count_nb_words(char *str, int i, int count)
{
	char	quote_c;

	while(str[i])
	{
		while(str[i] && str[i] == ' ') //skip espace depart
			i++;
		if (str[i] == '\'' || str[i] == '"')
		{
			quote_c = str[i++];
			while(str[i] && str[i] != quote_c)
				i++;
			count++;
			i++;
		}
		else
		{
			count++;
			while(str[i] && str[i] != ' ')
				i++;
		}
		while(str[i] && str[i] == ' ')
			i++;
	}
	return (count);
}

// char	*alloc_words(const char *s, unsigned int current_word)
// {
// 	char		*word;
// 	size_t		i;
// 	size_t		begin;
// 	size_t		end;

// 	i = 0;
// 	while (current_word--)
// 	{
// 		while (s[i] == c && s[i])
// 			i++;
// 		begin = i;
// 		while (s[i] != c && s[i])
// 			i++;
// 	}
// 	end = i;
// 	word = malloc(sizeof(char) * (end - begin + 1));
// 	if (word == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (begin < end)
// 		word[i++] = s[begin++];
// 	word[i] = '\0';
// 	return (word);
// }

// void	super_split(char *str)
// {
// 	char	**m_tab;
// 	size_t	count;
// 	int i;
// 	int	current_word = 1;

// 	i = 0;
// 	// printf("Nombre de mots %d : \n", count_nb_words(str, i, 0));
// 	count = count_nb_words(str, i, 0);
// 	tab = malloc(sizeof(char *) * (count + 1));
// 	if (tab == NULL)
// 		return (NULL);
// 	while (++current_word <= count)
// 	{
// 		tab[current_word] = alloc_words(str, current_word);
// 		if (tab[current_word] == NULL)
// 		{
// 			free_all(current_word + 1, tab);
// 			return (NULL);
// 		}
// 	}
// 	tab[count + 1] = NULL;
// }
//----------------------------------------------------
// char	*ft_read_input(t_shell *shell)
// {
//     char	*str;
// 	char	*path;

// 	path = path_string(shell);
//     str = readline(path);
// 	// super_split(str);
// 	free(path);
// 	if (str != NULL && str[0])
//     	add_history(str);
//     return(str);
// }
//----------------------------------------------------

size_t	ft_strlen_c(const char *str, char c, bool *found) //TO ADD
{
	size_t	i;

	i = 0;
	if (str == NULL)
		return (0);
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
	t_env_list *head;

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
		// (*new_var)->next = (*shell)->list;
		// (*shell)->list = (*new_var);  --> pour add en top de liste
		head = (*shell)->list;
		while(head->next)
			head = head->next;
		head->next = *new_var;
		(*new_var)->next = NULL;
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
	if (len == 0)  //si env -i
	{
		shell->env[0] = NULL;
		shell->list = NULL;
		return ; 
	} 
	while (i < len && len != 0)
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
	(void)signum;
	printf("\n");
	rl_replace_line("\0", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_signals(void)
{
	// rl_signal_event_hook = 
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler_c);
}

void printAST(t_ast_node* node)
{
    if (node == NULL)
		return;
	printf("\n\n*********PRINT********\n");
    printf("\nType: %d \n", node->type);
    printf("Command: %s \n", node->value);
	if (node->left)
		printf("At my left is : %s\n", node->left->value);
	if (node->right)
		printf("At my right is : %s\n", node->right->value);
    if (node->type == TOKEN_CMD)
    {
        if (node->args != NULL)
        {
            int i = 0;
            while (node->args[i] != NULL)
            {
                printf("Args: %s \n", node->args[i]);
                i++;
            }
        }
    }
    printf ("\n\n");
    printAST(node->left);
    printAST(node->right);
}

void	set_term_param()
{
	/////////////////
	//Il faut une librarie ..  --> sert pour ce qui serait graphique
	/////////////////
	// term = tgetent(NULL, getenv("TERM"));
	// if (term == -1)
    // {
    //     fprintf(stderr, "Could not access to the termcap database..\n");
    //     return -1;
    // }
    // else if (term == 0)
    // {
    //     fprintf(stderr, "Terminal type is not defined in termcap database (or have too few informations).\n");
    //     return -1;
    // }
	// exit(0);
	/////////////////
}

// char	*get_str_lenghts(char *s)
// {
// 	int	i;
// 	size_t	full_lght;
// 	size_t	dol_lght;
// 	bool	has_dollar;
	
// 	i = 0;
// 	dol_lght = 0;
// 	full_lght = 0;
// 	while(s[i++])
// 		full_lght++;
// 	i = 0;
// 	while(s[i])
// 	{
// 		if(s[i++] == '$')
// 		{
// 			has_dollar = true;
// 			dol_lght += 1;
// 			while(s[i] && s[i] != ' ' && s[i] != '\'' 
// 				&& s[i] != '"')
// 			{
// 				dol_lght += 1;
// 				i++;
// 			}
// 		}
// 	}
// 	if (has_dollar)
// 		s = replace_dollars(s, full_lght, dol_lght);
// 	return (s);
// }

int main (int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    // shell.exit = 0;
    // shell.exitcode = 0;
	// shell.heredoc_counter = 0;
	// shell.heredoc_name = NULL;
	ft_memset(&shell, 0, sizeof(shell));
    init_envp(&shell, envp);
    while(shell.exit == 0)
    {
		set_signals();
        shell.str = ft_read_input(&shell);
		// get_str_lenghts(shell.str);
		// split_arguments(shell.str, " \f\n\r\t\v");
        if (shell.str != NULL)
        {
            shell.list_token = lexer(shell.str);
            print_tokens(shell.list_token);
            shell.ast = build_ast(shell.list_token, true);
            printAST(shell.ast);
            execute_ast(shell.ast, &shell);
            free_ast(shell.ast);
            free_tokens(shell.list_token);
            free(shell.str);
        }
        else // -> Ctrl+D pressed : 
		{	
			printf("exit\n");
			rl_clear_history();
			shell.exit = 1;
		}
    }
	free(shell.heredoc_name);
	rl_clear_history();
	ft_free_split(shell.env); //free aussi le char **, etait enleve
    ft_free_env(&shell.list);
    return(0);
}
