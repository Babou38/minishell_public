/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:53:22 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/23 13:53:54 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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