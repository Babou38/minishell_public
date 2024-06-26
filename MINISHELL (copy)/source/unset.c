/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 13:50:21 by marvin            #+#    #+#             */
/*   Updated: 2024/04/14 13:50:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	delete_val_from_env(char **env, char *target, int *err)
{
	char	**env_ptr;
	char	**next_env_ptr;

	env_ptr = env;
	(*err) = 0;
	while (*env_ptr && ft_strncmp(*env_ptr, target, ft_strlen(target)) != 0)
		env_ptr++;
	if (*env_ptr)
	{
		free(*env_ptr);
		next_env_ptr = env_ptr;
		while (*next_env_ptr)
		{
			*next_env_ptr = *(next_env_ptr + 1);
			next_env_ptr++;
		}
	}
	else 
		(*err) = 1;
}

void ft_unset(t_shell *shell, t_ast_node *node)
{
	
	int	err;
	int		i;

	i = 1;
	err = 0;
	if (node->args[1] == NULL)
	{
		shell->exitcode = 0;
		return ;
	}
	while (node->args[i])
	{
		delete_val_from_env(shell->env, node->args[i], &err);
		if (err == 0)
			del_envlist(&shell, node->args[i]);
		i++;
	}
	shell->exitcode = 0; //toujours 0 ici ? 
}