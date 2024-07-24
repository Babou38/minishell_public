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

void	change_var_value(t_shell *shell, int i, int j, char **v_cmd)
{
	t_env_list	*head;
	char		*target;

	if (ft_strchr(v_cmd[j], '=') == NULL)
		return ;
	head = shell->list;
	target = get_var_name(v_cmd[j]);
	if (!target)
		safe_exit(shell, NULL, NONE, 1);
	while (ft_strcmp(head->var_name, target) != 0)
		head = head->next;
	free(target);
	if (head->value != NULL)
		free(head->value);
	head->value = get_var_value(v_cmd[j]);
	free(shell->env[i]);
	shell->env[i] = ft_strdup(v_cmd[j]);
}

void	add_to_export(t_shell *shell, char *new_var, char **v_cmd, int j)
{
	t_env_list	*new_list_node;
	char		**new_env;
	int			k;
	int			i;

	k = 0;
	i = -1;
	if (!var_already_exists(shell, &i, new_var))
	{
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
		shell->env = new_env;
		add_envlist(&shell, &new_list_node, new_var, false);
	}
	else
		change_var_value(shell, i, j, v_cmd);
}

void	ft_export(t_shell *shell, char **v_cmd)
{
	int	i;

	i = 1;
	if (v_cmd[1] == NULL)
		return (print_export(shell));
	while (v_cmd[i] != NULL)
	{
		if (var_is_valid(v_cmd[i]))
			add_to_export(shell, v_cmd[i], v_cmd, i);
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
