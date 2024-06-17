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

void ft_unset(t_shell *shell, t_ast_node *node)
{
    char	**env_ptr;
    char	**next_env_ptr;
	size_t	err;

	err = 0;
    env_ptr = shell->env;
	if (node->args[1] == NULL) // changer ailleurs
	{
		printf("Invalid syntax!\n");
		shell->exitcode = 1;
		return ;
	}
    while (*env_ptr && ft_strncmp(*env_ptr, node->args[1], ft_strlen(node->args[1])) != 0)
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
		err++;
	err += del_envlist(&shell, node->args[1]);
    shell->exitcode = err;
}