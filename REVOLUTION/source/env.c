/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:24:59 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:24:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_env(t_shell *shell) // TO ADD
{
	t_env_list *head;
    int i;
    i = 0;
	head = shell->list;
	if (head == NULL)
	{
		shell->exitcode = 125;
		return ;
	}
	while (head != NULL)
    {
		if (head->value)
        	printf("%s=%s\n", head->var_name, head->value);
		head = head->next;
        i++;
    }
    shell->exitcode = 0;
}
