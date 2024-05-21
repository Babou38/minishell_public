/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:24:19 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:24:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(t_env_list *list, char *target)
{
	t_env_list *head;
	char	*res;

	res = NULL;
	head = list;
	if (!target[0])
		return (res);
	while (head != NULL)
	{
		if (ft_strcmp(target, head->var_name) == 0)
		{
			//printf("found it ! -> %s = %s \n", head->var_name, head->value);
			res = ft_strdup(head->value);
			break;
		}
		else
			head = head->next;
	}
	return (res);
}

void ft_echo(t_shell *shell)
{
    int i = 1;
    int newline = 1;
    char *env_value;

    if (shell->cmd[i] != NULL && ft_strncmp(shell->cmd[i], "-n", 2) == 0) // plusieurs flags autorises
    {
        newline = 0;
        i++;
    }
    while (shell->cmd[i] != NULL)
    {
        if (ft_strcmp(shell->cmd[i], "$?") == 0)
            printf("%d", shell->exitcode);
        else if (shell->cmd[i][0] == '$')
        {
			env_value = ft_getenv(shell->list, shell->cmd[i] + 1);
        	if (env_value != NULL)
			{
                printf("%s", env_value);
				free(env_value);
			}
        }
        else
            printf("%s", shell->cmd[i]);
        i++;
    }
    if (newline)
        printf("\n");
    shell->exitcode = 0;
}