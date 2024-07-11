/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 14:19:02 by marvin            #+#    #+#             */
/*   Updated: 2024/04/14 14:19:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	var_already_exists(t_shell *shell, int *i, char *new)
{
	while (shell->env[++(*i)])
	{
		if (ft_strcmp_c(shell->env[*i], new, '=') == 0)
			return (1);
	}
	return (0);
}

void	print_export(t_shell *shell)
{
	t_env_list	*new_var;

	new_var = shell->list;
	while (new_var != NULL)
	{
	   	printf("declare -x %s", new_var->var_name);
		if (new_var->value)
			printf("=\"%s\"\n", new_var->value);
		else
			printf("\n");
		new_var = new_var->next;
	}
}

char	*get_var_name(const char *str)
{
	char	*name;
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*get_var_value(char *str)
{
	char	*value;

	value = str;
	while (value[0] != '=')
		value++;
	value++;
	// get_needed_str_size(value);
	value = ft_strdup(value);
	return (value);
}

void	change_var_value(t_shell *shell, int i, int j, t_ast_node *node)
{
	t_env_list	*head;
	char		*target;

	if (ft_strchr(node->args[j], '=') == NULL)// si pas de = , on touche pas -> marche peut etre pas ?
	{
		// printf("no = ..\n");
		return ;
	} 
	// else
	// 	printf("= was found ... changing ..\n");
	//SINON :
	head = shell->list;
	target = get_var_name(node->args[j]); //on prends le nom
	if (!target)
		safe_exit(shell, NULL, NONE, 1);
	while (ft_strcmp(head->var_name, target) != 0)
		head = head->next; //on bouge jusqu'a la var a changer
	free(target);
	if (head->value != NULL) // si on avait deja une valeur on la remplace
		free(head->value);
	head->value = get_var_value(node->args[j]);
	free(shell->env[i]);
	shell->env[i] = ft_strdup(node->args[j]); // change la var dans le char **
}

void	add_to_export(t_shell *shell, char *new_var, t_ast_node *node, int j)
{
	t_env_list	*new_list_node;
	char **new_env;
	int k;
	int i;

	k = 0;
	i = -1;
	if (!var_already_exists(shell, &i, new_var)) // si la var existe pas : 
	{
		// printf("doesn't exist ! adding ! \n");
		new_env = malloc(sizeof(char *) * (i + 2));
		if (!new_env)
			safe_exit(shell, NULL, NONE, 1);
		while (k < i)
		{
			new_env[k] = shell->env[k];
			k++;
		}
		new_env[i] = ft_strdup(new_var);
		new_env[i + 1] = NULL;
		free(shell->env);
		shell->env = new_env; //nouveau char ** pour env et on recopie tout
		add_envlist(&shell, &new_list_node, new_var, false); // ajout liste chainee
	}
	else
	{
		// printf("var exists ! modifiying ...\n");
		change_var_value(shell, i, j, node); //sinon, on regarde si on doit changer la value
	}
		
}

bool	var_is_valid(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[0]) || str[0] == '_')
	{
		while(str[i] && str[i] != '=') // on s'arrete au = je suppose ?
		{
			if (str[i] < 48 || (str[i] >= 58 && str[i] <= 64) 
			|| (str[i] >= 91 && str[i] <= 96 )
			|| (str[i] > 122))
			{
				if (str[i] != '_' && str[i] != ' ')
				{
					// printf("false because of %c ...\n", str[i]);
					return (false);
				}
			}
			i++;
		}
		return (true);
	}
	return (false);
}

void ft_export(t_shell *shell, t_ast_node *node)
{
	int i;
	
	i = 1;
	if (node->args[1] == NULL)
		return (print_export(shell));
	while (node->args[i] != NULL)
	{
		// printf("s[%d] : %s\n", i,node->args[i]);
		if (var_is_valid(node->args[i]))
			add_to_export(shell, node->args[i], node, i);
		else
		{
			printferr(" not a valid identifier\n");
			shell->exitcode = 1;
			return ;
		}
		i++;
	}
	shell->exitcode = 0;
}