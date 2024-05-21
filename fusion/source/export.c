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

int	var_already_exists(t_shell *shell, int *i)
{
	while (shell->env[++(*i)])
	{
		if (ft_strcmp_c(shell->env[*i], shell->ast->args[1], '=') == 0)
			return (1);
		//on compare le string jusqu'au signe egal
	}
	return (0);
}

t_env_list	*print_export(t_shell *shell)
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
	
	return (new_var);
}

char	*get_var_name(const char *str)
{
	char	*name;
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(i + 1);
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
	value = ft_strdup(value);
	return (value);
}

void	change_var_value(t_shell *shell, int i)
{
	t_env_list	*head;
	char		*target;

	if (ft_strchr(shell->ast->args[1], '=') == NULL) // si pas de = , on touche pas
		return ;
	//SINON :
	head = shell->list;
	target = get_var_name(shell->ast->args[1]); //on prends le nom
	while (ft_strcmp(head->var_name, target) != 0)
		head = head->next; //on bouge jusqu'a la var a changer
	free(target);
	if (head->value != NULL) // si on avait deja une valeur on la remplace
		free(head->value);
	head->value = get_var_value(shell->ast->args[1]);
	free(shell->env[i]);
	shell->env[i] = ft_strdup(shell->ast->args[1]); // change la var dans le char **
}

void ft_export(t_shell *shell)
{
	int i;
	int k;
	char **new_env;
	t_env_list	*new_var;

	i = -1;
	k = 0;
	if (shell->ast->args[1] == NULL) // si juste "export"
	{
		new_var = print_export(shell);
		return ;
	}
	if (!var_already_exists(shell, &i)) // si la var existe pas : 
	{
		new_env = malloc(sizeof(char *) * (i + 2));
		while (k < i)
		{
			new_env[k] = shell->env[k];
			k++;
		}
		new_env[i] = ft_strdup(shell->ast->args[1]);
		new_env[i + 1] = NULL;
		free(shell->env);
		shell->env = new_env; //nouveau char ** pour env et on recopie tout
		add_envlist(&shell, &new_var, shell->ast->args[1], false); // ajout liste chainee
	}
	else
		change_var_value(shell, i); //sinon, on regarde si on doit changer la value
	shell->exitcode = 0;
}