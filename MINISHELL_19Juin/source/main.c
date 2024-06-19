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

// int	count_nb_words(char *str, int i, int count)
// {
// 	char	quote_c;

// 	while(str[i])
// 	{
// 		while(str[i] && str[i] == ' ') //skip espace depart
// 			i++;
// 		if (str[i] == '\'' || str[i] == '"')
// 		{
// 			quote_c = str[i++];
// 			while(str[i] && str[i] != quote_c)
// 				i++;
// 			count++;
// 			i++;
// 		}
// 		else
// 		{
// 			count++;
// 			while(str[i] && str[i] != ' ')
// 				i++;
// 		}
// 		while(str[i] && str[i] == ' ')
// 			i++;
// 	}
// 	return (count);
// }

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
		(*new_var) = malloc(sizeof(t_env_list));
		if (!(*new_var))
			safe_exit(*shell, NULL, NONE, 1);
		(*new_var)->var_name = ft_strdup_c(env, &j, '=', &found);
		if (!(*new_var)->var_name)
			safe_exit(*shell, (*new_var), NODE, 1);
		env += j;
		if (found == false)
			(*new_var)->value = NULL;
		else
			(*new_var)->value = ft_strdup_c(env, &j, '\0', &found);
		if (!(*new_var)->value)
			safe_exit(*shell, (*new_var), NODE, 1);
		head = (*shell)->list;
		while(head->next)
			head = head->next;
		head->next = *new_var;
		(*new_var)->next = NULL;
	}
	else
	{
		(*shell)->list = malloc(sizeof(t_env_list));
		if (!((*shell)->list))
			safe_exit(*shell, NULL, NONE, 1);
		(*shell)->list->var_name = ft_strdup_c(env, &j, '=', &found);
		if (!(*shell)->list->var_name)
			safe_exit(*shell, (*shell)->list, NODE, 1);
		env += j;
		if (found == false)
			(*shell)->list->value = NULL;
		else
			(*shell)->list->value = ft_strdup_c(env, &j, '\0', &found);
		if (!(*shell)->list->value)
			safe_exit(*shell, (*shell)->list, NODE, 1);
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
	shell->env = malloc((len + 1) * sizeof(char *)); // to check
	if (len == 0)  //si env -i
	{
		shell->env[0] = NULL;
		shell->list = NULL;
		return ; 
	}
	while (i < len && len != 0)
	{
		shell->env[i] = ft_strdup(envp[i]); //remplissage char ** /// to check
		i++;
	}
	shell->env[len] = NULL;
	i = 0; // 0 et pas -1 car la premiere var est deja mise juste la : 
	add_envlist(&shell, &new_var, envp[0], true); // ajout tete de la liste chainee
	while (++i < len)
		add_envlist(&shell, &new_var, envp[i], false); // rempli liste
	
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


int main (int argc, char **argv, char **envp)
{
	t_shell shell;
	bool	is_valid;

	(void)argc;
	(void)argv;
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
			shell.list_token = lexer(shell.str, &is_valid);
			if (!is_valid)
				safe_exit(&shell, NULL, NONE, 1);
			print_tokens(shell.list_token);
			shell.ast = build_ast(shell.list_token, true);
			if (!shell.ast)
				safe_exit(&shell, NULL, NONE, 1);
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
