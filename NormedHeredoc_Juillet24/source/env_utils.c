/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 11:15:14 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/19 11:15:15 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	del_envlist(t_shell **shell, char *name)
{
	t_env_list	*var;
	t_env_list	*new;

	var = (*shell)->list;
	if (ft_strcmp(name, var->var_name) == 0)
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

void	new_to_list(t_shell **shell, t_env_list **new_node, char *var)
{
	bool		found;
	int			j;
	t_env_list	*head;

	j = 0;
	found = false;
	(*new_node) = malloc(sizeof(t_env_list));
	if (!(*new_node))
		safe_exit(*shell, NULL, NONE, 1);
	(*new_node)->var_name = ft_strdup_c(var, &j, '=', &found);
	if (!(*new_node)->var_name)
		safe_exit(*shell, (*new_node), NODE, 1);
	var += j;
	if (found == false)
		(*new_node)->value = NULL;
	else
		(*new_node)->value = ft_strdup_c(var, &j, '\0', &found);
	if ((!(*new_node)->value) && found == true)
		safe_exit(*shell, (*new_node), NODE, 1);
	head = (*shell)->list;
	while (head->next)
		head = head->next;
	head->next = *new_node;
	(*new_node)->next = NULL;
}

void	add_envlist(t_shell **shell, t_env_list **new_node,
	char *var, bool lone)
{
	int			j;
	bool		found;

	j = 0;
	found = false;
	if (lone == false)
		new_to_list(shell, new_node, var);
	else
	{
		(*shell)->list = malloc(sizeof(t_env_list));
		if (!((*shell)->list))
			safe_exit(*shell, NULL, NONE, 1);
		(*shell)->list->var_name = ft_strdup_c(var, &j, '=', &found);
		if (!(*shell)->list->var_name)
			safe_exit(*shell, (*shell)->list, NODE, 1);
		var += j;
		if (found == false)
			(*shell)->list->value = NULL;
		else
			(*shell)->list->value = ft_strdup_c(var, &j, '\0', &found);
		if (!(*shell)->list->value)
			safe_exit(*shell, (*shell)->list, NODE, 1);
		(*shell)->list->next = NULL;
	}
}

bool	env_is_empty(t_shell *shell, int len)
{
	if (len == 0)
	{
		shell->env[0] = NULL;
		shell->list = NULL;
		return (true);
	}
	return (false);
}
